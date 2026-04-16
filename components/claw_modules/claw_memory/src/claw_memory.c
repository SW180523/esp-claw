/*
 * SPDX-FileCopyrightText: 2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include "claw_memory.h"

#include <inttypes.h>
#include <stdbool.h>
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "esp_log.h"

static const char *TAG = "claw_memory";
static const char *DEFAULT_LONG_TERM_MEMORY =
    "# Long-term Memory\n\n"
    "(empty - ESP-Claw will write memories here as it learns)\n";

#define CLAW_MEMORY_DEFAULT_MAX_SESSION_MESSAGES 20
#define CLAW_MEMORY_DEFAULT_MAX_MESSAGE_CHARS    1024

typedef struct {
    int initialized;
    char session_root_dir[128];
    char long_term_memory_path[128];
    size_t max_session_messages;
    size_t max_message_chars;
} claw_memory_state_t;

static claw_memory_state_t s_memory = {0};

static void safe_copy(char *dst, size_t dst_size, const char *src)
{
    size_t len;

    if (!dst || dst_size == 0) {
        return;
    }
    if (!src) {
        dst[0] = '\0';
        return;
    }

    len = strnlen(src, dst_size - 1);
    memcpy(dst, src, len);
    dst[len] = '\0';
}

static char *alloc_zeroed(size_t size)
{
    if (size == 0) {
        return NULL;
    }
    return calloc(1, size);
}

static char *dup_printf(const char *fmt, ...)
{
    va_list args;
    va_list copy;
    int needed;
    char *buf;

    va_start(args, fmt);
    va_copy(copy, args);
    needed = vsnprintf(NULL, 0, fmt, copy);
    va_end(copy);
    if (needed < 0) {
        va_end(args);
        return NULL;
    }

    buf = calloc(1, (size_t)needed + 1);
    if (!buf) {
        va_end(args);
        return NULL;
    }

    vsnprintf(buf, (size_t)needed + 1, fmt, args);
    va_end(args);
    return buf;
}

static size_t utf8_sequence_len(unsigned char ch)
{
    if (ch < 0x80) {
        return 1;
    }
    if ((ch & 0xE0) == 0xC0) {
        return 2;
    }
    if ((ch & 0xF0) == 0xE0) {
        return 3;
    }
    if ((ch & 0xF8) == 0xF0) {
        return 4;
    }
    return 0;
}

static bool utf8_sequence_valid(const unsigned char *src, size_t len)
{
    size_t i;

    if (!src || len == 0) {
        return false;
    }
    for (i = 1; i < len; i++) {
        if (src[i] == '\0') {
            return false;
        }
        if ((src[i] & 0xC0) != 0x80) {
            return false;
        }
    }
    return true;
}

static size_t text_buffer_size(size_t max_chars)
{
    if (max_chars == 0) {
        max_chars = CLAW_MEMORY_DEFAULT_MAX_MESSAGE_CHARS;
    }
    return (max_chars * 4) + 1;
}

static void normalize_text(const char *src, char *dst, size_t dst_size, size_t max_chars)
{
    size_t off = 0;
    size_t chars = 0;

    if (!dst || dst_size == 0) {
        return;
    }
    dst[0] = '\0';
    if (!src) {
        return;
    }

    while (*src && off + 1 < dst_size && chars < max_chars) {
        const unsigned char *cur = (const unsigned char *)src;
        size_t seq_len = utf8_sequence_len(*cur);

        if (*cur < 0x80) {
            char ch = (char) * cur++;
            src = (const char *)cur;
            if (ch == '\r' || ch == '\n' || ch == '\t') {
                ch = ' ';
            }
            dst[off++] = ch;
            chars++;
            continue;
        }

        if (seq_len == 0 || !utf8_sequence_valid(cur, seq_len)) {
            src++;
            continue;
        }
        if (off + seq_len >= dst_size) {
            break;
        }
        memcpy(dst + off, cur, seq_len);
        off += seq_len;
        src += seq_len;
        chars++;
    }
    dst[off] = '\0';
}

static void sanitize_session_id(const char *session_id, char *buf, size_t size)
{
    size_t off = 0;

    if (!buf || size == 0) {
        return;
    }
    buf[0] = '\0';
    if (!session_id) {
        return;
    }

    while (*session_id && off + 1 < size) {
        char ch = *session_id++;
        if (isalnum((unsigned char)ch) || ch == '-' || ch == '_') {
            buf[off++] = ch;
        } else {
            buf[off++] = '_';
        }
    }
    buf[off] = '\0';
}

static esp_err_t ensure_parent_dir(const char *path)
{
    char *tmp = NULL;
    char *slash;
    esp_err_t err = ESP_FAIL;

    if (!path || path[0] == '\0') {
        return ESP_ERR_INVALID_ARG;
    }

    tmp = strdup(path);
    if (!tmp) {
        return ESP_ERR_NO_MEM;
    }
    slash = strrchr(tmp, '/');
    if (!slash) {
        free(tmp);
        return ESP_OK;
    }
    *slash = '\0';
    if (tmp[0] == '\0') {
        free(tmp);
        return ESP_OK;
    }
    {
        struct stat st;
        if (stat(tmp, &st) == 0 && S_ISDIR(st.st_mode)) {
            free(tmp);
            return ESP_OK;
        }
    }
    if (mkdir(tmp, 0755) == 0 || errno == EEXIST) {
        free(tmp);
        return ESP_OK;
    }
    {
        char *probe_path = dup_printf("%s/.claw_memory_probe", tmp);
        FILE *probe;

        probe = probe_path ? fopen(probe_path, "w") : NULL;
        if (probe) {
            fclose(probe);
            remove(probe_path);
            err = ESP_OK;
        }
        free(probe_path);
    }
    free(tmp);
    return err;
}

static esp_err_t ensure_dir(const char *path)
{
    struct stat st;
    char *probe_path = NULL;
    FILE *probe = NULL;

    if (!path || path[0] == '\0') {
        return ESP_ERR_INVALID_ARG;
    }
    if (stat(path, &st) == 0 && S_ISDIR(st.st_mode)) {
        return ESP_OK;
    }
    if (mkdir(path, 0755) == 0 || errno == EEXIST) {
        return ESP_OK;
    }
    probe_path = dup_printf("%s/.claw_memory_probe", path);
    if (!probe_path) {
        return ESP_ERR_NO_MEM;
    }
    probe = fopen(probe_path, "w");
    if (probe) {
        fclose(probe);
        remove(probe_path);
        free(probe_path);
        return ESP_OK;
    }
    free(probe_path);
    return ESP_FAIL;
}

static char *session_path_dup(const char *session_id)
{
    char safe_session_id[48];
    uint32_t hash = 2166136261u;
    const unsigned char *p = (const unsigned char *)session_id;
    size_t len;
    char *path;

    sanitize_session_id(session_id, safe_session_id, sizeof(safe_session_id));
    for (; p && *p; ++p) {
        hash ^= *p;
        hash *= 16777619u;
    }

    len = strnlen(safe_session_id, sizeof(safe_session_id) - 1);
    if (len > 24) {
        safe_session_id[24] = '\0';
    }

    path = dup_printf("%s/s_%s_%08" PRIx32 ".log",
                      s_memory.session_root_dir,
                      safe_session_id[0] ? safe_session_id : "default",
                      hash);
    return path;
}

static void json_escape(const char *src, char *dst, size_t dst_size)
{
    size_t off = 0;

    if (!dst || dst_size == 0) {
        return;
    }
    dst[0] = '\0';
    if (!src) {
        return;
    }

    while (*src && off + 1 < dst_size) {
        char ch = *src++;
        if (ch == '"' || ch == '\\') {
            if (off + 2 >= dst_size) {
                break;
            }
            dst[off++] = '\\';
            dst[off++] = ch;
        } else {
            dst[off++] = ch;
        }
    }
    dst[off] = '\0';
}

static esp_err_t append_line(FILE *file, const char *role, const char *text)
{
    char *normalized = NULL;
    size_t max_chars = s_memory.max_message_chars;
    size_t normalized_size;
    esp_err_t err = ESP_OK;

    if (!file || !role || !text) {
        return ESP_ERR_INVALID_ARG;
    }

    if (max_chars > CLAW_MEMORY_DEFAULT_MAX_MESSAGE_CHARS) {
        max_chars = CLAW_MEMORY_DEFAULT_MAX_MESSAGE_CHARS;
    }
    normalized_size = text_buffer_size(max_chars);
    normalized = alloc_zeroed(normalized_size);
    if (!normalized) {
        return ESP_ERR_NO_MEM;
    }
    normalize_text(text, normalized, normalized_size, max_chars);
    fprintf(file, "%s\t%s\n", role, normalized);
    free(normalized);
    return err;
}

static esp_err_t ensure_long_term_memory_file(const char *path)
{
    FILE *file;

    if (!path || path[0] == '\0') {
        return ESP_ERR_INVALID_ARG;
    }

    file = fopen(path, "r");
    if (file) {
        fclose(file);
        return ESP_OK;
    }

    file = fopen(path, "w");
    if (!file) {
        return ESP_FAIL;
    }

    if (fputs(DEFAULT_LONG_TERM_MEMORY, file) < 0) {
        fclose(file);
        return ESP_FAIL;
    }

    fclose(file);
    return ESP_OK;
}

static size_t session_history_json_size(void)
{
    size_t max_msgs = s_memory.max_session_messages;
    size_t max_chars = s_memory.max_message_chars;

    if (max_msgs == 0) {
        max_msgs = CLAW_MEMORY_DEFAULT_MAX_SESSION_MESSAGES;
    }
    if (max_chars == 0 || max_chars > CLAW_MEMORY_DEFAULT_MAX_MESSAGE_CHARS) {
        max_chars = CLAW_MEMORY_DEFAULT_MAX_MESSAGE_CHARS;
    }

    return (max_msgs * ((text_buffer_size(max_chars) * 2) + 64)) + 16;
}

static esp_err_t read_file_dup(const char *path, char **out_buf)
{
    FILE *file = NULL;
    long size = 0;
    char *buf = NULL;
    size_t read_bytes;

    if (!path || !out_buf) {
        return ESP_ERR_INVALID_ARG;
    }
    *out_buf = NULL;

    file = fopen(path, "rb");
    if (!file) {
        return ESP_ERR_NOT_FOUND;
    }
    if (fseek(file, 0, SEEK_END) != 0) {
        fclose(file);
        return ESP_FAIL;
    }
    size = ftell(file);
    if (size < 0) {
        fclose(file);
        return ESP_FAIL;
    }
    if (fseek(file, 0, SEEK_SET) != 0) {
        fclose(file);
        return ESP_FAIL;
    }

    buf = calloc(1, (size_t)size + 1);
    if (!buf) {
        fclose(file);
        return ESP_ERR_NO_MEM;
    }

    read_bytes = fread(buf, 1, (size_t)size, file);
    fclose(file);
    buf[read_bytes] = '\0';
    *out_buf = buf;
    return ESP_OK;
}

static esp_err_t claw_memory_session_load_json(const char *session_id,
                                               char *buf,
                                               size_t size)
{
    char *path = NULL;
    FILE *file = NULL;
    char *line = NULL;
    char **roles = NULL;
    char **contents = NULL;
    char *escaped = NULL;
    size_t count = 0;
    size_t next = 0;
    size_t i;
    size_t max_msgs;
    size_t max_chars;
    size_t max_bytes;
    size_t line_size;
    size_t off = 0;

    if (!s_memory.initialized || !session_id || !buf || size == 0) {
        return ESP_ERR_INVALID_STATE;
    }

    max_msgs = s_memory.max_session_messages;
    max_chars = s_memory.max_message_chars;
    if (max_msgs == 0) {
        max_msgs = CLAW_MEMORY_DEFAULT_MAX_SESSION_MESSAGES;
    }
    if (max_chars == 0 || max_chars > CLAW_MEMORY_DEFAULT_MAX_MESSAGE_CHARS) {
        max_chars = CLAW_MEMORY_DEFAULT_MAX_MESSAGE_CHARS;
    }
    max_bytes = text_buffer_size(max_chars) - 1;

    path = session_path_dup(session_id);
    line_size = max_bytes + 16;
    line = alloc_zeroed(line_size);
    escaped = alloc_zeroed((max_bytes * 2) + 1);
    if (!path || !line || !escaped) {
        free(path);
        free(line);
        free(escaped);
        return ESP_ERR_NO_MEM;
    }

    file = fopen(path, "r");
    if (!file) {
        buf[0] = '\0';
        free(path);
        free(line);
        free(escaped);
        return ESP_ERR_NOT_FOUND;
    }

    roles = calloc(max_msgs, sizeof(char *));
    contents = calloc(max_msgs, sizeof(char *));
    if (!roles || !contents) {
        fclose(file);
        free(path);
        free(line);
        free(escaped);
        free(roles);
        free(contents);
        return ESP_ERR_NO_MEM;
    }

    for (i = 0; i < max_msgs; i++) {
        roles[i] = calloc(1, 16);
        contents[i] = calloc(1, max_bytes + 1);
        if (!roles[i] || !contents[i]) {
            fclose(file);
            free(path);
            free(line);
            free(escaped);
            for (i = 0; i < max_msgs; i++) {
                free(roles[i]);
                free(contents[i]);
            }
            free(roles);
            free(contents);
            return ESP_ERR_NO_MEM;
        }
    }

    while (fgets(line, (int)line_size, file)) {
        char *tab = strchr(line, '\t');
        char *value;
        if (!tab) {
            ESP_LOGW(TAG, "Skipping malformed session line for %s: %.32s",
                     session_id, line);
            continue;
        }
        *tab = '\0';
        value = tab + 1;
        value[strcspn(value, "\r\n")] = '\0';
        safe_copy(roles[next], 16, line);
        safe_copy(contents[next], max_bytes + 1, value);
        next = (next + 1) % max_msgs;
        if (count < max_msgs) {
            count++;
        }
    }
    fclose(file);
    free(path);

    buf[0] = '\0';
    off += snprintf(buf + off, size - off, "[");
    for (i = 0; i < count && off + 1 < size; i++) {
        size_t idx = (count < max_msgs) ? i : ((next + i) % max_msgs);
        const char *role = strcmp(roles[idx], "assistant") == 0 ? "assistant" : "user";

        escaped[0] = '\0';
        json_escape(contents[idx], escaped, (max_bytes * 2) + 1);
        off += snprintf(buf + off, size - off,
                        "%s{\"role\":\"%s\",\"content\":\"%s\"}",
                        (i == 0) ? "" : ",",
                        role,
                        escaped);
    }
    if (off + 2 <= size) {
        snprintf(buf + off, size - off, "]");
    } else if (size > 0) {
        buf[size - 1] = '\0';
    }

    for (i = 0; i < max_msgs; i++) {
        free(roles[i]);
        free(contents[i]);
    }
    free(roles);
    free(contents);
    free(line);
    free(escaped);

    return ESP_OK;
}

esp_err_t claw_memory_init(const claw_memory_config_t *config)
{
    if (!config || !config->session_root_dir || !config->long_term_memory_path) {
        return ESP_ERR_INVALID_ARG;
    }

    memset(&s_memory, 0, sizeof(s_memory));
    safe_copy(s_memory.session_root_dir, sizeof(s_memory.session_root_dir), config->session_root_dir);
    safe_copy(s_memory.long_term_memory_path, sizeof(s_memory.long_term_memory_path), config->long_term_memory_path);
    s_memory.max_session_messages = config->max_session_messages ? config->max_session_messages :
                                    CLAW_MEMORY_DEFAULT_MAX_SESSION_MESSAGES;
    s_memory.max_message_chars = config->max_message_chars ? config->max_message_chars :
                                 CLAW_MEMORY_DEFAULT_MAX_MESSAGE_CHARS;

    if (ensure_dir(s_memory.session_root_dir) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to create session root: %s", s_memory.session_root_dir);
        return ESP_FAIL;
    }
    if (ensure_parent_dir(s_memory.long_term_memory_path) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to prepare long-term memory path: %s", s_memory.long_term_memory_path);
        return ESP_FAIL;
    }
    if (ensure_long_term_memory_file(s_memory.long_term_memory_path) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to prepare long-term memory file: %s", s_memory.long_term_memory_path);
        return ESP_FAIL;
    }

    s_memory.initialized = 1;
    ESP_LOGI(TAG, "Initialized");
    return ESP_OK;
}

esp_err_t claw_memory_session_load(const char *session_id, char *buf, size_t size)
{
    return claw_memory_session_load_json(session_id, buf, size);
}

esp_err_t claw_memory_session_append(const char *session_id,
                                     const char *user_text,
                                     const char *assistant_text)
{
    char *path = NULL;
    FILE *file = NULL;
    esp_err_t err = ESP_OK;

    if (!s_memory.initialized || !session_id || !user_text || !assistant_text) {
        return ESP_ERR_INVALID_STATE;
    }

    path = session_path_dup(session_id);
    if (!path) {
        return ESP_ERR_NO_MEM;
    }
    file = fopen(path, "a");
    if (!file) {
        ESP_LOGE(TAG, "Failed to open session file: %s", path);
        free(path);
        return ESP_FAIL;
    }

    err = append_line(file, "user", user_text);
    if (err == ESP_OK) {
        err = append_line(file, "assistant", assistant_text);
    }
    fclose(file);
    free(path);
    return err;
}

esp_err_t claw_memory_long_term_read(char *buf, size_t size)
{
    FILE *file = NULL;
    size_t n;

    if (!s_memory.initialized || !buf || size == 0) {
        return ESP_ERR_INVALID_STATE;
    }

    file = fopen(s_memory.long_term_memory_path, "r");
    if (!file) {
        buf[0] = '\0';
        return ESP_ERR_NOT_FOUND;
    }

    n = fread(buf, 1, size - 1, file);
    buf[n] = '\0';
    fclose(file);
    return ESP_OK;
}

esp_err_t claw_memory_long_term_write(const char *content)
{
    FILE *file = NULL;

    if (!s_memory.initialized || !content) {
        return ESP_ERR_INVALID_STATE;
    }

    file = fopen(s_memory.long_term_memory_path, "w");
    if (!file) {
        return ESP_FAIL;
    }
    fputs(content, file);
    fclose(file);
    return ESP_OK;
}

esp_err_t claw_memory_append_session_turn_callback(const char *session_id,
                                                   const char *user_text,
                                                   const char *assistant_text,
                                                   void *user_ctx)
{
    (void)user_ctx;
    return claw_memory_session_append(session_id, user_text, assistant_text);
}

static esp_err_t claw_memory_long_term_collect(const claw_core_request_t *request,
                                               claw_core_context_t *out_context,
                                               void *user_ctx)
{
    char *content = NULL;
    esp_err_t err;

    (void)request;
    (void)user_ctx;

    if (!out_context) {
        return ESP_ERR_INVALID_ARG;
    }

    memset(out_context, 0, sizeof(*out_context));
    err = read_file_dup(s_memory.long_term_memory_path, &content);
    if (err != ESP_OK) {
        return err;
    }
    if (!content[0]) {
        free(content);
        return ESP_ERR_NOT_FOUND;
    }

    out_context->kind = CLAW_CORE_CONTEXT_KIND_SYSTEM_PROMPT;
    out_context->content = content;
    return ESP_OK;
}

static esp_err_t claw_memory_session_history_collect(const claw_core_request_t *request,
                                                     claw_core_context_t *out_context,
                                                     void *user_ctx)
{
    char *content = NULL;
    size_t content_size;
    esp_err_t err;

    (void)user_ctx;

    if (!request || !out_context || !request->session_id || !request->session_id[0]) {
        return ESP_ERR_NOT_FOUND;
    }

    memset(out_context, 0, sizeof(*out_context));
    content_size = session_history_json_size();
    content = calloc(1, content_size);
    if (!content) {
        return ESP_ERR_NO_MEM;
    }

    err = claw_memory_session_load_json(request->session_id, content, content_size);
    if (err != ESP_OK) {
        free(content);
        return err;
    }
    if (!content[0] || strcmp(content, "[]") == 0) {
        free(content);
        return ESP_ERR_NOT_FOUND;
    }

    out_context->kind = CLAW_CORE_CONTEXT_KIND_MESSAGES;
    out_context->content = content;
    return ESP_OK;
}

const claw_core_context_provider_t claw_memory_long_term_provider = {
    .name = "Long-term Memory",
    .collect = claw_memory_long_term_collect,
    .user_ctx = NULL,
};

const claw_core_context_provider_t claw_memory_session_history_provider = {
    .name = "Session History",
    .collect = claw_memory_session_history_collect,
    .user_ctx = NULL,
};
