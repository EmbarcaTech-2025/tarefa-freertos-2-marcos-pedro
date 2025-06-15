#ifndef _NOISE_DETECTOR_H_
#define _NOISE_DETECTOR_H_

#include "main.h"

/**
* @brief  When the system is armed, this task is unsuspended. 
* It continuously samples the Electret Microphone. 
* If the sound level passes a threshold, it triggers the alarm.
**/
void vTaskNoiseDetector(void * pvParameters);

#endif