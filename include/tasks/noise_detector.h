#ifndef _NOISE_DETECTOR_H_
#define _NOISE_DETECTOR_H_

#include "main.h"
#include "math.h"

#define DC_OFFSET 2048 
#define SENSITIVITY_THRESHOLD 400
#define SAMPLES 200

#define abs(x) ((x < 0) ? (-x) : (x))

/**
* @brief Returns the Root Mean Squared of the samples
**/
float mic_power();

/**
* @brief  When the system is armed, this task is unsuspended. 
* It continuously samples the Electret Microphone. 
* If the sound level passes a threshold, it triggers the alarm.
**/
void vTaskNoiseDetector(void * pvParameters);

#endif