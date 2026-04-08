#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "cap_scheduler.h"
#include "cJSON.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"

#define CAP_SCHEDULER_DEFAULT_SCHEDULES_PATH "/fatfs/data/scheduler/schedules.json"
#define CAP_SCHEDULER_DEFAULT_STATE_PATH     "/fatfs/data/scheduler/scheduler_state.json"
#define CAP_SCHEDULER_DEFAULT_TIMEZONE       "UTC0"
#define CAP_SCHEDULER_DEFAULT_TICK_MS        1000
#define CAP_SCHEDULER_DEFAULT_MAX_ITEMS      32
#define CAP_SCHEDULER_DEFAULT_STACK          6144
#define CAP_SCHEDULER_DEFAULT_PRIORITY       5

typedef struct {
    bool occupied;
    cap_scheduler_item_t item;
    cap_scheduler_status_t status;
    int64_t next_fire_ms;
    int64_t last_fire_ms;
    int64_t last_success_ms;
    int run_count;
    int missed_count;
    esp_err_t last_error_code;
} cap_scheduler_entry_t;

typedef struct {
    bool initialized;
    bool started;
    bool stop_requested;
    SemaphoreHandle_t mutex;
    TaskHandle_t task_handle;
    cap_scheduler_config_t config;
    char schedules_path[192];
    char state_path[192];
    char default_timezone[CAP_SCHEDULER_TZ_LEN];
    cap_scheduler_entry_t *entries;
    size_t max_items;
    size_t item_count;
} cap_scheduler_runtime_t;

extern cap_scheduler_runtime_t s_cap_scheduler;

int64_t cap_scheduler_now_ms(void);
void cap_scheduler_apply_defaults(cap_scheduler_item_t *item, const char *default_timezone);
bool cap_scheduler_is_supported_timezone(const char *timezone);
esp_err_t cap_scheduler_validate_item(const cap_scheduler_item_t *item);
esp_err_t cap_scheduler_compute_next_fire(const cap_scheduler_item_t *item,
                                          int64_t anchor_ms,
                                          int run_count,
                                          int64_t *out_next_fire_ms);
esp_err_t cap_scheduler_load_items(const char *path,
                                   cap_scheduler_item_t *items,
                                   size_t max_items,
                                   size_t *out_count,
                                   const char *default_timezone);
esp_err_t cap_scheduler_parse_item_json_string(const char *json,
                                               cap_scheduler_item_t *item,
                                               const char *default_timezone);
esp_err_t cap_scheduler_save_items(const char *path,
                                   const cap_scheduler_entry_t *entries,
                                   size_t entry_count);
esp_err_t cap_scheduler_load_state(const char *path,
                                   cap_scheduler_entry_t *entries,
                                   size_t entry_count);
esp_err_t cap_scheduler_save_state(const char *path,
                                   const cap_scheduler_entry_t *entries,
                                   size_t entry_count);
esp_err_t cap_scheduler_entry_to_json(const cap_scheduler_entry_t *entry,
                                      bool include_item,
                                      cJSON **out_json);
