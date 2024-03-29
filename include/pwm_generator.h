/**
 * @file pwm_generator.h
 * @author Halit Cetin (halit.cetin@alten.com)
 * @brief This file includes PWM generator definitions.
 * @version 0.1
 * @date 2022-06-14
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef PWM_GENERATOR_H
#define PWM_GENERATOR_H

#include <mbed.h>

#include "variables.h"
#include "serial_output.h"

/**
 * @brief Used for deciding if the PWM generator task rising the PWM 
 * period or not.
 * 
 */
enum PWM_PERIOD_DIR
{
    ERROR_DIR,
    RISING_DIR,
    FALLING_DIR
};

/**
 * @brief Set the PWM output pins.
 * 
 * @param led_pin LED PWM pin
 * @param cable_pin Measurement PWM pin
 */
void pwm_generator_set_pins(PinName led_pin, PinName cable_pin);

/**
 * @brief PWM generator thread.
 * 
 * @details This thread is responsible from generating sample RPM signals
 * as PWM. It is important that we are not changing the duty cycle. We are 
 * changing the period of the PWM signal.
 */
void pwm_generator_thread();

/**
 * @brief Starts the related thread.
 * 
 */
void pwm_generator_start_thread();

#endif 