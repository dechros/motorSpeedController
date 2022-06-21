/**
 * @file pwm_capturing.h
 * @author Halit Cetin (halit.cetin@alten.com)
 * @brief This file includes PWM capturing definitions.
 * @version 0.1
 * @date 2022-06-17
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef PWM_CAPTURING_H
#define PWM_CAPTURING_H

#include <mbed.h>

#include "variables.h"
#include "serial_output.h"

/**
 * @brief Set the PWM input pin.
 * 
 * @param led_pin Input PWM pin
 */
void pwm_capturing_set_pin(PinName input_pin);

/**
 * @brief Starts the related thread.
 * 
 */
void pwm_capturing_start_thread();

/**
 * @brief Interrupt function for PWM input.
 * 
 */
void pwm_input_rise();

/**
 * @brief PWM capture thread.
 * 
 * @details This thread is responsible from capturing sample RPM signals
 * as PWM.
 */
void pwm_capturing_thread();

#endif