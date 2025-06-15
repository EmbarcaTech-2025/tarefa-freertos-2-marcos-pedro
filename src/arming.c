#include "arming.h"

void vTaskArming(void *pvParameters) {
    for(;;) {
        ssd1306_clear(disp);
        ssd1306_draw_string(disp, 32, 32, 1, "ABC");
        ssd1306_show(disp);
        vTaskDelay(pdMS_TO_TICKS(1000));
        ssd1306_clear(disp);
        ssd1306_draw_string(disp, 32, 32, 1, "ABD");
        ssd1306_show(disp);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}