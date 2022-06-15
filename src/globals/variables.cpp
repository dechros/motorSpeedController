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

volatile bool interrupted = false;

PwmOut led(LED1);
PwmOut pwm(D3);
InterruptIn input(D8);
BufferedSerial serial(USBTX, USBRX);