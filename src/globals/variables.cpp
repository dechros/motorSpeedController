/**
 * @file variables.cpp
 * @author Halit Cetin (halit.cetin@alten.com)
 * @brief This file includes global variable definitions.
 * @version 0.1
 * @date 2022-06-14
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "globals/variables.h"

int period_ms = 0;
volatile bool interrupted = false;

LED_DIRECTION led_dir = RISING_DIR;
PwmOut led(LED1);
PwmOut pwm(D3);
InterruptIn input(D8);
BufferedSerial serial(USBTX, USBRX);