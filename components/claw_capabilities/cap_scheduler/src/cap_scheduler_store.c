#include "cap_scheduler_internal.h"

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

static esp_err_t cap_scheduler_read_file(const char *path, char **out_buf)
{
    FILE *file = NULL;
    long file_size;
    char *buf = NULL;

    if (!path || !out_buf) {
        return ESP_ERR_INVALID_ARG;
    }

    file = fopen(path, "rb");
    if (!file) {
        return errno == ENOENT ? ESP_ERR_NOT_FOUND : ESP_FAIL;
    }

    if (fseek(file, 0, SEEK_END) != 0) {
        fclose(file);
        return ESP_FAIL;
    }
    file_size = ftell(file);
    if (file_size < 0) {
        fclose(file);
        return ESP_FAIL;
    }
    rewind(file);

    buf = calloc(1, (size_t)file_size + 1);
    if (!buf) {
        fclose(file);
        return ESP_ERR_NO_MEM;
    }
    if (file_size > 0 && fread(buf, 1, (size_t)file_size, file) != (size_t)file_size) {
        fclose(file);
        free(buf);
        return ESP_FAIL;
    }

    fclose(file);
    *out_buf = buf;
    return ESP_OK;
}

static esp_err_t cap_scheduler_ensure_parent_dir(const char *path)
{
    char buf[256];
    char *cursor = NULL;
    char *slash = NULL;
    char *create_from = NULL;
    struct stat st = {0};
    size_t len;

    if (!path || !path[0]) {
        return ESP_ERR_INVALID_ARG;
    }

    strlcpy(buf, path, sizeof(buf));
    slash = strrchr(buf, '/');
    if (!slash) {
        return ESP_OK;
    }
    *slash = '\0';
    len = strlen(buf);

    for (cursor = buf + 1; *cursor; cursor++) {
        if (*cursor != '/') {
            continue;
        }
        *cursor = '\0';
        if (stat(buf, &st) == 0 && S_ISDIR(st.st_mode)) {
            create_from = cursor + 1;
        }
        *cursor = '/';
    }

    if (!create_from) {
        return ESP_FAIL;
    }

    for (cursor = create_from; *cursor; cursor++) {
        if (*cursor != '/') {
            continue;
        }
        *cursor = '\0';
        if (mkdir(buf, 0755) != 0 && errno != EEXIST) {
            return ESP_FAIL;
        }
        *cursor = '/';
    }

    if (len > 0 && mkdir(buf, 0755) != 0 && errno != EEXIST) {
        return ESP_FAIL;
    }
    return ESP_OK;
}

static esp_err_t cap_scheduler_write_file(const char *path, const char *content)
{
    FILE *file = NULL;

    if (!path || !content) {
        return ESP_ERR_INVALID_ARG;
    }
    if (cap_scheduler_ensure_parent_dir(path) != ESP_OK) {
        return ESP_FAIL;
    }

    file = fopen(path, "wb");
    if (!file) {
        return ESP_FAIL;
    }
    if (fwrite(content, 1, strlen(content), file) != strlen(content)) {
        fclose(file);
        return ESP_FAIL;
    }
    fclose(file);
    return ESP_OK;
}

static char *cap_scheduler_normalize_payload_json_field(const char *json)
{
    const char *field = "\"payload_json\"";
    const char *field_pos = NULL;
    const char *cursor = NULL;
    const char *value_quote = NULL;
    const char *obj_start = NULL;
    const char *obj_end = NULL;
    const char *closing_quote = NULL;
    bool in_string = false;
    bool escape = false;
    int depth = 0;
    size_t prefix_len;
    size_t obj_len;
    size_t suffix_len;
    char *normalized = NULL;

    if (!json) {
        return NULL;
    }

    field_pos = strstr(json, field);
    if (!field_pos) {
        return NULL;
    }

    cursor = field_pos + strlen(field);
    while (*cursor && isspace((unsigned char)*cursor)) {
        cursor++;
    }
    if (*cursor != ':') {
        return NULL;
    }
    cursor++;
    while (*cursor && isspace((unsigned char)*cursor)) {
        cursor++;
    }
    if (*cursor != '"') {
        return NULL;
    }

    value_quote = cursor;
    cursor++;
    if (*cursor != '{') {
        return NULL;
    }

    obj_start = cursor;
    for (; *cursor; cursor++) {
        char ch = *cursor;

        if (in_string) {
            if (escape) {
                escape = false;
            } else if (ch == '\\') {
                escape = true;
            } else if (ch == '"') {
                in_string = false;
            }
            continue;
        }

        if (ch == '"') {
            in_string = true;
            continue;
        }
        if (ch == '{') {
            depth++;
            continue;
        }
        if (ch == '}') {
            depth--;
            if (depth == 0) {
                obj_end = cursor;
                break;
            }
        }
    }

    if (!obj_end || depth != 0) {
        return NULL;
    }

    closing_quote = obj_end + 1;
    if (*closing_quote != '"') {
        return NULL;
    }

    prefix_len = (size_t)(value_quote - json);
    obj_len = (size_t)(obj_end - obj_start + 1);
    suffix_len = strlen(closing_quote + 1);

    normalized = calloc(1, prefix_len + obj_len + suffix_len + 1);
    if (!normalized) {
        return NULL;
    }

    memcpy(normalized, json, prefix_len);
    memcpy(normalized + prefix_len, obj_start, obj_len);
    memcpy(normalized + prefix_len + obj_len, closing_quote + 1, suffix_len);
    return normalized;
}

static const char *cap_scheduler_kind_to_string(cap_scheduler_item_kind_t kind)
{
    switch (kind) {
    case CAP_SCHEDULER_ITEM_ONCE:
        return "once";
    case CAP_SCHEDULER_ITEM_INTERVAL:
        return "interval";
    case CAP_SCHEDULER_ITEM_CRON:
        return "cron";
    default:
        return "unknown";
    }
}

static bool cap_scheduler_kind_from_string(const char *value, cap_scheduler_item_kind_t *out)
{
    if (!value || !out) {
        return false;
    }
    if (strcmp(value, "once") == 0) {
        *out = CAP_SCHEDULER_ITEM_ONCE;
        return true;
    }
    if (strcmp(value, "interval") == 0) {
        *out = CAP_SCHEDULER_ITEM_INTERVAL;
        return true;
    }
    if (strcmp(value, "cron") == 0) {
        *out = CAP_SCHEDULER_ITEM_CRON;
        return true;
    }
    return false;
}

static const char *cap_scheduler_status_to_string(cap_scheduler_status_t status)
{
    switch (status) {
    case CAP_SCHEDULER_STATUS_SCHEDULED:
        return "scheduled";
    case CAP_SCHEDULER_STATUS_PAUSED:
        return "paused";
    case CAP_SCHEDULER_STATUS_RUNNING:
        return "running";
    case CAP_SCHEDULER_STATUS_COMPLETED:
        return "completed";
    case CAP_SCHEDULER_STATUS_ERROR:
        return "error";
    case CAP_SCHEDULER_STATUS_DISABLED:
        return "disabled";
    default:
        return "unknown";
    }
}

static bool cap_scheduler_status_from_string(const char *value, cap_scheduler_status_t *out)
{
    if (!value || !out) {
        return false;
    }
    if (strcmp(value, "scheduled") == 0) {
        *out = CAP_SCHEDULER_STATUS_SCHEDULED;
        return true;
    }
    if (strcmp(value, "paused") == 0) {
        *out = CAP_SCHEDULER_STATUS_PAUSED;
        return true;
    }
    if (strcmp(value, "running") == 0) {
        *out = CAP_SCHEDULER_STATUS_RUNNING;
        return true;
    }
    if (strcmp(value, "completed") == 0) {
        *out = CAP_SCHEDULER_STATUS_COMPLETED;
        return true;
    }
    if (strcmp(value, "error") == 0) {
        *out = CAP_SCHEDULER_STATUS_ERROR;
        return true;
    }
    if (strcmp(value, "disabled") == 0) {
        *out = CAP_SCHEDULER_STATUS_DISABLED;
        return true;
    }
    return false;
}

static const char *cap_scheduler_catch_up_to_string(cap_scheduler_catch_up_policy_t policy)
{
    return policy == CAP_SCHEDULER_CATCH_UP_FIRE_ONCE ? "fire_once" : "skip";
}

static cap_scheduler_catch_up_policy_t cap_scheduler_catch_up_from_string(const char *value)
{
    if (value && strcmp(value, "fire_once") == 0) {
        return CAP_SCHEDULER_CATCH_UP_FIRE_ONCE;
    }
    return CAP_SCHEDULER_CATCH_UP_SKIP;
}

static void cap_scheduler_parse_item_json(const cJSON *node,
                                          cap_scheduler_item_t *item,
                                          const char *default_timezone)
{
    const cJSON *value;

    memset(item, 0, sizeof(*item));
    item->enabled = true;
    item->catch_up_policy = CAP_SCHEDULER_CATCH_UP_SKIP;

    value = cJSON_GetObjectItemCaseSensitive(node, "id");
    if (cJSON_IsString(value)) {
        strlcpy(item->id, value->valuestring, sizeof(item->id));
    }
    value = cJSON_GetObjectItemCaseSensitive(node, "enabled");
    if (cJSON_IsBool(value)) {
        item->enabled = cJSON_IsTrue(value);
    }
    value = cJSON_GetObjectItemCaseSensitive(node, "kind");
    if (cJSON_IsString(value)) {
        cap_scheduler_kind_from_string(value->valuestring, &item->kind);
    }
    value = cJSON_GetObjectItemCaseSensitive(node, "timezone");
    if (cJSON_IsString(value)) {
        strlcpy(item->timezone, value->valuestring, sizeof(item->timezone));
    }
    value = cJSON_GetObjectItemCaseSensitive(node, "start_at_ms");
    if (cJSON_IsNumber(value)) {
        item->start_at_ms = (int64_t)value->valuedouble;
    }
    value = cJSON_GetObjectItemCaseSensitive(node, "end_at_ms");
    if (cJSON_IsNumber(value)) {
        item->end_at_ms = (int64_t)value->valuedouble;
    }
    value = cJSON_GetObjectItemCaseSensitive(node, "interval_ms");
    if (cJSON_IsNumber(value)) {
        item->interval_ms = (int64_t)value->valuedouble;
    }
    value = cJSON_GetObjectItemCaseSensitive(node, "cron_expr");
    if (cJSON_IsString(value)) {
        strlcpy(item->cron_expr, value->valuestring, sizeof(item->cron_expr));
    }
    value = cJSON_GetObjectItemCaseSensitive(node, "event_type");
    if (cJSON_IsString(value)) {
        strlcpy(item->event_type, value->valuestring, sizeof(item->event_type));
    }
    value = cJSON_GetObjectItemCaseSensitive(node, "event_key");
    if (cJSON_IsString(value)) {
        strlcpy(item->event_key, value->valuestring, sizeof(item->event_key));
    }
    value = cJSON_GetObjectItemCaseSensitive(node, "source_channel");
    if (cJSON_IsString(value)) {
        strlcpy(item->source_channel, value->valuestring, sizeof(item->source_channel));
    }
    value = cJSON_GetObjectItemCaseSensitive(node, "chat_id");
    if (cJSON_IsString(value)) {
        strlcpy(item->chat_id, value->valuestring, sizeof(item->chat_id));
    }
    value = cJSON_GetObjectItemCaseSensitive(node, "content_type");
    if (cJSON_IsString(value)) {
        strlcpy(item->content_type, value->valuestring, sizeof(item->content_type));
    }
    value = cJSON_GetObjectItemCaseSensitive(node, "session_policy");
    if (cJSON_IsString(value)) {
        strlcpy(item->session_policy, value->valuestring, sizeof(item->session_policy));
    }
    value = cJSON_GetObjectItemCaseSensitive(node, "text");
    if (cJSON_IsString(value)) {
        strlcpy(item->text, value->valuestring, sizeof(item->text));
    }
    value = cJSON_GetObjectItemCaseSensitive(node, "payload_json");
    if (cJSON_IsString(value)) {
        strlcpy(item->payload_json, value->valuestring, sizeof(item->payload_json));
    } else if (cJSON_IsObject(value)) {
        char *rendered = cJSON_PrintUnformatted((cJSON *)value);

        if (rendered) {
            strlcpy(item->payload_json, rendered, sizeof(item->payload_json));
            free(rendered);
        }
    }
    value = cJSON_GetObjectItemCaseSensitive(node, "catch_up_policy");
    if (cJSON_IsString(value)) {
        item->catch_up_policy = cap_scheduler_catch_up_from_string(value->valuestring);
    }
    value = cJSON_GetObjectItemCaseSensitive(node, "max_runs");
    if (cJSON_IsNumber(value)) {
        item->max_runs = value->valueint;
    }

    cap_scheduler_apply_defaults(item, default_timezone);
}

esp_err_t cap_scheduler_entry_to_json(const cap_scheduler_entry_t *entry,
                                      bool include_item,
                                      cJSON **out_json)
{
    cJSON *root = NULL;

    if (!entry || !out_json) {
        return ESP_ERR_INVALID_ARG;
    }

    root = cJSON_CreateObject();
    if (!root) {
        return ESP_ERR_NO_MEM;
    }

    cJSON_AddStringToObject(root, "id", entry->item.id);
    cJSON_AddStringToObject(root, "status", cap_scheduler_status_to_string(entry->status));
    cJSON_AddNumberToObject(root, "next_fire_ms", (double)entry->next_fire_ms);
    cJSON_AddNumberToObject(root, "last_fire_ms", (double)entry->last_fire_ms);
    cJSON_AddNumberToObject(root, "last_success_ms", (double)entry->last_success_ms);
    cJSON_AddNumberToObject(root, "run_count", entry->run_count);
    cJSON_AddNumberToObject(root, "missed_count", entry->missed_count);
    cJSON_AddNumberToObject(root, "last_error_code", entry->last_error_code);

    if (include_item) {
        cJSON_AddBoolToObject(root, "enabled", entry->item.enabled);
        cJSON_AddStringToObject(root, "kind", cap_scheduler_kind_to_string(entry->item.kind));
        cJSON_AddStringToObject(root, "timezone", entry->item.timezone);
        cJSON_AddNumberToObject(root, "start_at_ms", (double)entry->item.start_at_ms);
        cJSON_AddNumberToObject(root, "end_at_ms", (double)entry->item.end_at_ms);
        cJSON_AddNumberToObject(root, "interval_ms", (double)entry->item.interval_ms);
        cJSON_AddStringToObject(root, "cron_expr", entry->item.cron_expr);
        cJSON_AddStringToObject(root, "event_type", entry->item.event_type);
        cJSON_AddStringToObject(root, "event_key", entry->item.event_key);
        cJSON_AddStringToObject(root, "source_channel", entry->item.source_channel);
        cJSON_AddStringToObject(root, "chat_id", entry->item.chat_id);
        cJSON_AddStringToObject(root, "content_type", entry->item.content_type);
        cJSON_AddStringToObject(root, "session_policy", entry->item.session_policy);
        cJSON_AddStringToObject(root, "text", entry->item.text);
        cJSON_AddStringToObject(root, "payload_json", entry->item.payload_json);
        cJSON_AddStringToObject(root, "catch_up_policy", cap_scheduler_catch_up_to_string(entry->item.catch_up_policy));
        cJSON_AddNumberToObject(root, "max_runs", entry->item.max_runs);
    }

    *out_json = root;
    return ESP_OK;
}

esp_err_t cap_scheduler_load_items(const char *path,
                                   cap_scheduler_item_t *items,
                                   size_t max_items,
                                   size_t *out_count,
                                   const char *default_timezone)
{
    char *buf = NULL;
    cJSON *root = NULL;
    size_t count = 0;
    esp_err_t err;

    if (!items || !out_count) {
        return ESP_ERR_INVALID_ARG;
    }

    err = cap_scheduler_read_file(path, &buf);
    if (err == ESP_ERR_NOT_FOUND) {
        *out_count = 0;
        return ESP_OK;
    }
    if (err != ESP_OK) {
        return err;
    }

    root = cJSON_Parse(buf);
    free(buf);
    if (!cJSON_IsArray(root)) {
        cJSON_Delete(root);
        return ESP_ERR_INVALID_RESPONSE;
    }

    cJSON *node = NULL;
    cJSON_ArrayForEach(node, root) {
        if (!cJSON_IsObject(node)) {
            continue;
        }
        if (count >= max_items) {
            cJSON_Delete(root);
            return ESP_ERR_NO_MEM;
        }
        cap_scheduler_parse_item_json(node, &items[count], default_timezone);
        err = cap_scheduler_validate_item(&items[count]);
        if (err != ESP_OK) {
            cJSON_Delete(root);
            return err;
        }
        count++;
    }

    cJSON_Delete(root);
    *out_count = count;
    return ESP_OK;
}

esp_err_t cap_scheduler_parse_item_json_string(const char *json,
                                               cap_scheduler_item_t *item,
                                               const char *default_timezone)
{
    char *normalized_json = NULL;
    char *fallback_json = NULL;
    const char *parse_json = NULL;
    const char *start = NULL;
    const char *end = NULL;
    cJSON *root = NULL;
    esp_err_t err;

    if (!json || !item) {
        return ESP_ERR_INVALID_ARG;
    }

    start = json;
    while (*start && isspace((unsigned char)*start)) {
        start++;
    }
    end = start + strlen(start);
    while (end > start && isspace((unsigned char)end[-1])) {
        end--;
    }

    parse_json = start;
    if ((end - start) >= 2 &&
        ((start[0] == '\'' && end[-1] == '\'') || (start[0] == '"' && end[-1] == '"'))) {
        size_t json_len = (size_t)(end - start - 2);

        normalized_json = calloc(1, json_len + 1);
        if (!normalized_json) {
            return ESP_ERR_NO_MEM;
        }
        memcpy(normalized_json, start + 1, json_len);
        parse_json = normalized_json;
    }

    root = cJSON_Parse(parse_json);
    if (!root) {
        fallback_json = cap_scheduler_normalize_payload_json_field(parse_json);
        if (fallback_json) {
            root = cJSON_Parse(fallback_json);
        }
    }
    free(fallback_json);
    free(normalized_json);
    if (!cJSON_IsObject(root)) {
        cJSON_Delete(root);
        return ESP_ERR_INVALID_ARG;
    }

    cap_scheduler_parse_item_json(root, item, default_timezone);
    cJSON_Delete(root);

    err = cap_scheduler_validate_item(item);
    if (err != ESP_OK) {
        return err;
    }
    return ESP_OK;
}

esp_err_t cap_scheduler_save_items(const char *path,
                                   const cap_scheduler_entry_t *entries,
                                   size_t entry_count)
{
    cJSON *root = cJSON_CreateArray();
    char *rendered = NULL;
    esp_err_t err = ESP_OK;

    if (!root) {
        return ESP_ERR_NO_MEM;
    }

    for (size_t i = 0; i < entry_count; i++) {
        cJSON *obj = NULL;

        if (!entries[i].occupied) {
            continue;
        }
        err = cap_scheduler_entry_to_json(&entries[i], true, &obj);
        if (err != ESP_OK) {
            cJSON_Delete(root);
            return err;
        }
        cJSON_DeleteItemFromObject(obj, "status");
        cJSON_DeleteItemFromObject(obj, "next_fire_ms");
        cJSON_DeleteItemFromObject(obj, "last_fire_ms");
        cJSON_DeleteItemFromObject(obj, "last_success_ms");
        cJSON_DeleteItemFromObject(obj, "run_count");
        cJSON_DeleteItemFromObject(obj, "missed_count");
        cJSON_DeleteItemFromObject(obj, "last_error_code");
        cJSON_AddItemToArray(root, obj);
    }

    rendered = cJSON_Print(root);
    cJSON_Delete(root);
    if (!rendered) {
        return ESP_ERR_NO_MEM;
    }
    err = cap_scheduler_write_file(path, rendered);
    free(rendered);
    return err;
}

esp_err_t cap_scheduler_load_state(const char *path,
                                   cap_scheduler_entry_t *entries,
                                   size_t entry_count)
{
    char *buf = NULL;
    cJSON *root = NULL;
    esp_err_t err;

    if (!entries) {
        return ESP_ERR_INVALID_ARG;
    }

    err = cap_scheduler_read_file(path, &buf);
    if (err == ESP_ERR_NOT_FOUND) {
        return ESP_OK;
    }
    if (err != ESP_OK) {
        return err;
    }

    root = cJSON_Parse(buf);
    free(buf);
    if (!cJSON_IsArray(root)) {
        cJSON_Delete(root);
        return ESP_ERR_INVALID_RESPONSE;
    }

    cJSON *node = NULL;
    cJSON_ArrayForEach(node, root) {
        const cJSON *id_item = cJSON_GetObjectItemCaseSensitive(node, "id");
        if (!cJSON_IsString(id_item)) {
            continue;
        }
        for (size_t i = 0; i < entry_count; i++) {
            const cJSON *value;

            if (!entries[i].occupied || strcmp(entries[i].item.id, id_item->valuestring) != 0) {
                continue;
            }
            value = cJSON_GetObjectItemCaseSensitive(node, "status");
            if (cJSON_IsString(value)) {
                cap_scheduler_status_from_string(value->valuestring, &entries[i].status);
            }
            value = cJSON_GetObjectItemCaseSensitive(node, "next_fire_ms");
            if (cJSON_IsNumber(value)) {
                entries[i].next_fire_ms = (int64_t)value->valuedouble;
            }
            value = cJSON_GetObjectItemCaseSensitive(node, "last_fire_ms");
            if (cJSON_IsNumber(value)) {
                entries[i].last_fire_ms = (int64_t)value->valuedouble;
            }
            value = cJSON_GetObjectItemCaseSensitive(node, "last_success_ms");
            if (cJSON_IsNumber(value)) {
                entries[i].last_success_ms = (int64_t)value->valuedouble;
            }
            value = cJSON_GetObjectItemCaseSensitive(node, "run_count");
            if (cJSON_IsNumber(value)) {
                entries[i].run_count = value->valueint;
            }
            value = cJSON_GetObjectItemCaseSensitive(node, "missed_count");
            if (cJSON_IsNumber(value)) {
                entries[i].missed_count = value->valueint;
            }
            value = cJSON_GetObjectItemCaseSensitive(node, "last_error_code");
            if (cJSON_IsNumber(value)) {
                entries[i].last_error_code = value->valueint;
            }
            break;
        }
    }

    cJSON_Delete(root);
    return ESP_OK;
}

esp_err_t cap_scheduler_save_state(const char *path,
                                   const cap_scheduler_entry_t *entries,
                                   size_t entry_count)
{
    cJSON *root = cJSON_CreateArray();
    char *rendered = NULL;
    esp_err_t err = ESP_OK;

    if (!root) {
        return ESP_ERR_NO_MEM;
    }

    for (size_t i = 0; i < entry_count; i++) {
        cJSON *obj = NULL;

        if (!entries[i].occupied) {
            continue;
        }
        err = cap_scheduler_entry_to_json(&entries[i], false, &obj);
        if (err != ESP_OK) {
            cJSON_Delete(root);
            return err;
        }
        cJSON_AddItemToArray(root, obj);
    }

    rendered = cJSON_Print(root);
    cJSON_Delete(root);
    if (!rendered) {
        return ESP_ERR_NO_MEM;
    }
    err = cap_scheduler_write_file(path, rendered);
    free(rendered);
    return err;
}
