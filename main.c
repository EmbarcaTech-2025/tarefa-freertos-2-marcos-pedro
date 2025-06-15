#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#define STACK_SIZE 256
typedef void (*Multitask)(void*);

void led_task() {
  const uint LED_PIN = 11;
  gpio_init(LED_PIN);
  gpio_set_dir(LED_PIN, GPIO_OUT);
  while (true) {
    gpio_put(LED_PIN, 1);
    vTaskDelay(100);
    gpio_put(LED_PIN, 0);
    vTaskDelay(100);
  }
}

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

  vCreateFunction("LED_Task", led_task);
  vTaskStartScheduler();

  while(1){};
}