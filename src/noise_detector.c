#include "noise_detector.h"

void vTaskNoiseDetector(void * pvParameters) {
    for(;;) {
        printf("ABC\n");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}