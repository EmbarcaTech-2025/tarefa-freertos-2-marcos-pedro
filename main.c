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

TaskHandle_t vCreateFunction(char * name, Multitask func)
{
    BaseType_t xReturned;
    TaskHandle_t xHandle = NULL;

    xReturned = xTaskCreate(
                    func,      
                    name,         
                    STACK_SIZE,     
                    ( void * ) 1,    
                    tskIDLE_PRIORITY,
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

  alarm_handle = vCreateFunction("alarm_task", vTaskAlarm);
  arming_handle = vCreateFunction("arming_task", vTaskArming);
  noise_detector_handle = vCreateFunction("noise_detector_task", vTaskNoiseDetector);
  vTaskStartScheduler();

  while(1){};
}