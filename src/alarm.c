#include "alarm.h"

void vTaskAlarm(void * pvParameters) {
    for(;;) {
        vTaskDelay(pdMS_TO_TICKS(5000));
        vTaskSuspend(arming_handle);
        vTaskDelay(pdMS_TO_TICKS(5000));
        vTaskResume(arming_handle);
    }
}