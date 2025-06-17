#include "alarm.h"

void vTaskAlarm(void * pvParameters) {
    xSemaphoreTake(xNoiseDetectedSemaphore, portMAX_DELAY);

    for(;;) {
        uint slice_num_a = pwm_gpio_to_slice_num(BUZZER_A_PIN);
        uint channel_num_a = pwm_gpio_to_channel(BUZZER_A_PIN);

        uint slice_num_b = pwm_gpio_to_slice_num(BUZZER_B_PIN);
        uint channel_num_b = pwm_gpio_to_channel(BUZZER_B_PIN);

        npClear();
        for(uint8_t i=0; i<MATRIX_LED_COUNT; i++)
            npSetLED(i, 1, 0, 0);
        npWrite();
        pwm_set_chan_level(slice_num_a, channel_num_a, 255);
        pwm_set_chan_level(slice_num_b, channel_num_b, 255);
        ssd1306_clear_square(disp, 0, 32, 128, 8);
        ssd1306_draw_string(disp, 8, 32, 1, "LOUD NOISE DETECTED!");
        vTaskDelay(pdMS_TO_TICKS(100));
        npClear();
        npWrite();
        pwm_set_chan_level(slice_num_a, channel_num_a, 0);
        pwm_set_chan_level(slice_num_b, channel_num_b, 0);
        ssd1306_clear_square(disp, 0, 32, 128, 8);

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}