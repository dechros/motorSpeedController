/**
 * @file variables.h
 * @author Halit Cetin (halit.cetin@alten.com)
 * @brief This file includes global variable declarations.
 * @version 0.1
 * @date 2022-06-14
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef VARIABLES_H
#define VARIABLES_H

#include <mbed.h>

extern volatile bool interrupted;

extern PwmOut led;
extern PwmOut pwm;
extern InterruptIn input;
extern BufferedSerial serial;

#endif