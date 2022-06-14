/**
 * @file variables.h
 * @author Halit Cetin (halit.cetin@alten.com)
 * @brief This file includes global variables.
 * @version 0.1
 * @date 2022-06-14
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <mbed.h>

#include "../globals/enums.h"
#include "../pwm_generator/pwm_generator.h"

int period_ms;
volatile bool interrupted;

LED_DIRECTION led_dir;
PwmOut led(LED1);
PwmOut pwm(D3);
InterruptIn input(D8);
BufferedSerial serial(USBTX, USBRX);
Thread pwm_generator_thread;
PwmGenerator pwm_generator;