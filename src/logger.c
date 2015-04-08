#include <pebble_worker.h>
  
#define DATA_LOG_TAG_ACC 53
#define SAMPLE_BATCH 10
  
static DataLoggingSessionRef s_log_ref;

static void accel_data_handler(AccelData *data, uint32_t num_samples) {
    if (num_samples >= SAMPLE_BATCH) {
        DataLoggingResult r = data_logging_log(s_log_ref, data, 1);
      if (r != DATA_LOGGING_SUCCESS) {
        APP_LOG(APP_LOG_LEVEL_ERROR, "Error datalogging.");
      }
    }
}
static void worker_init() {
   // Open new log
    s_log_ref = data_logging_create(DATA_LOG_TAG_ACC, DATA_LOGGING_BYTE_ARRAY, sizeof(AccelData) * SAMPLE_BATCH, false);

    // Start generating data
    accel_data_service_subscribe(SAMPLE_BATCH, &accel_data_handler);
    accel_service_set_sampling_rate(ACCEL_SAMPLING_10HZ);
}

static void worker_deinit() {
  accel_data_service_unsubscribe();
  data_logging_finish(s_log_ref);
}

int main(void) {
  worker_init();
  worker_event_loop();
  worker_deinit();
}