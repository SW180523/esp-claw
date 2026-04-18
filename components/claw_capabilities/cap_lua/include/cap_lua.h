/*
 * SPDX-FileCopyrightText: 2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "esp_err.h"
#include "lua.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    const char *name;
    lua_CFunction open_fn;
} cap_lua_module_t;

typedef void (*cap_lua_runtime_cleanup_fn_t)(void);

const char *cap_lua_get_base_dir(void);
esp_err_t cap_lua_register_group(void);
esp_err_t cap_lua_set_base_dir(const char *base_dir);
esp_err_t cap_lua_register_module(const char *name, lua_CFunction open_fn);
esp_err_t cap_lua_register_modules(const cap_lua_module_t *modules, size_t count);
esp_err_t cap_lua_register_runtime_cleanup(cap_lua_runtime_cleanup_fn_t cleanup_fn);
esp_err_t cap_lua_list_scripts(const char *prefix, char *output, size_t output_size);
esp_err_t cap_lua_write_script(const char *path, const char *content, bool overwrite, char *output, size_t output_size);
esp_err_t cap_lua_run_script(const char *path, const char *args_json, uint32_t timeout_ms, char *output, size_t output_size);
esp_err_t cap_lua_run_script_async(const char *path, const char *args_json, uint32_t timeout_ms, char *output, size_t output_size);
esp_err_t cap_lua_list_jobs(const char *status, char *output, size_t output_size);
esp_err_t cap_lua_get_job(const char *job_id, char *output, size_t output_size);

#ifdef __cplusplus
}
#endif
