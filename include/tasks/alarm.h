#ifndef _ALARM_H_
#define _ALARM_H_

#include "main.h"

/**
* @brief  This task is normally suspended. 
* When triggered by the Task_NoiseDetector, 
* it flashes all the colors on the common cathode RGB LED 
* and the 5x5 RGB Matrix while making 
* an annoying noise with both buzzers
**/
void vTaskAlarm(void * pvParameters);

#endif