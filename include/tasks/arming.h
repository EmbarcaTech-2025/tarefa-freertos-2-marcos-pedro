#ifndef _ARMING_H_
#define _ARMING_H_

#include "main.h"

/**
* @brief  This task manages the state (disarmed, arming, armed). 
* It shows "Enter Code" on the OLED display and 
* reads the Analog Joystick for the pattern.
**/
void vTaskArming(void *pvParameters);

#endif