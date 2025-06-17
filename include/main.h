#ifndef _MAIN_H
#define _MAIN_H

#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware_manager.h"

extern ssd1306_t *disp;
extern TaskHandle_t alarm_handle;
extern TaskHandle_t arming_handle;
extern TaskHandle_t noise_detector_handle;

extern int direction_arr[8];
extern int answer_arr[8];

#endif