/**
 * @file pwm_capturing.cpp
 * @author Halit Cetin (halit.cetin@alten.com)
 * @brief This file includes PWM capturing related declerations.
 * @version 0.1
 * @date 2022-06-17
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "pwm_capturing.h"

Thread pwm_capture_thread;
Mutex pwm_capture_mutex;
DigitalIn *pwm_in = NULL;