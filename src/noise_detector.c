#include "noise_detector.h"

uint16_t adc_buffer[SAMPLES];

float mic_power()
{
  float avg = 0.f;

  for (uint i = 0; i < SAMPLES; ++i) {
    float temp = adc_buffer[i] - DC_OFFSET;
    avg += temp * temp;
  }

  avg /= SAMPLES;
  return sqrt(avg);
}

void vTaskNoiseDetector(void *pvParameters)
{
  ssd1306_clear(disp);
  ssd1306_draw_string(disp, 0, 32, 1, "DON'T MAKE LOUD NOISES");
  ssd1306_show(disp);

  for (;;)
  {
    adc_gpio_init(MIC_PIN);
    adc_select_input(MIC_CHANNEL);

    for (int i = 0; i < SAMPLES; i++)
      adc_buffer[i] = adc_read();

    float avg = mic_power();   
    npClear(); 
    if(avg >= SENSITIVITY_THRESHOLD) {
      xSemaphoreGive(xNoiseDetectedSemaphore);
      vTaskSuspend(NULL);
    }     
    npWrite();

    vTaskDelay(pdMS_TO_TICKS(10));
  }
}