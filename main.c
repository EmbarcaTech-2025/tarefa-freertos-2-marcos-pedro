// Tasks
#include "alarm.h"
#include "noise_detector.h"
#include "arming.h"

#define STACK_SIZE 256
typedef void (*Multitask)(void*);

ssd1306_t *disp;
TaskHandle_t alarm_handle;
TaskHandle_t arming_handle;
TaskHandle_t noise_detector_handle;

SemaphoreHandle_t xArmedSemaphore;
SemaphoreHandle_t xNoiseDetectedSemaphore;

TaskHandle_t vCreateFunction(char * name, Multitask func, uint priority)
{
    BaseType_t xReturned;
    TaskHandle_t xHandle = NULL;

    xReturned = xTaskCreate(
                    func,      
                    name,         
                    STACK_SIZE,     
                    ( void * ) 1,    
                    priority,
                    &xHandle );     

    return xHandle;
}

int main() {
  stdio_init_all();
  ssd1306_t temp;
  bool success = init_hardware(&temp);
  if (!success)
  {
    printf("Failed to init display. Program finished\n");
    return 0;
  }
  disp = &temp;

  xArmedSemaphore = xSemaphoreCreateBinary();
  xNoiseDetectedSemaphore = xSemaphoreCreateBinary();

  alarm_handle = vCreateFunction("alarm_task", vTaskAlarm, 1);
  arming_handle = vCreateFunction("arming_task", vTaskArming, 2);
  noise_detector_handle = vCreateFunction("noise_detector_task", vTaskNoiseDetector, 3);
  vTaskStartScheduler();

  while(1){};
}