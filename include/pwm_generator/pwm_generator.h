/**
 * @file pwm_generator.h
 * @author Halit Cetin (halit.cetin@alten.com)
 * @brief This file includes PWM Generator class related definitions.
 * @version 0.1
 * @date 2022-06-14
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef PWM_GENERATOR_H
#define PWM_GENERATOR_H

#include <mbed.h>

#include "globals/variables.h"
#include "serial_output/serial_output.h"

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
 * @brief Generates two same PWM signals in two different pins. One is 
 * used for driving LED and other one is used for measuring purposes.
 * 
 */
class PwmGenerator
{
private:
    Thread thread;
    static PwmOut *led_out;
    static PwmOut *pwm_out;
    static SerialOutput *serial;

    /**
     * @brief PWM generator thread.
     * 
     * @details This thread is responsible from generating sample RPM signals
     * as PWM. It is important that we are not changing the duty cycle. We are 
     * changing the period of the PWM signal.
     */
    static void pwm_generator_thread();

public: 
    /**
     * @brief Construct a new PWM Generator object.
     * 
     */
    PwmGenerator();

    /**
     * @brief Destroy the PWM Generator object.
     * 
     */
    ~PwmGenerator();

    /**
     * @brief Set the PWM output pins.
     * 
     * @param led_pin Designated LED pin for visual
     * @param output_pin Designated PWM pin for measurement
     */
    void set_pins(PinName led_pin, PinName output_pin);

    /**
     * @brief Set the serial output object.
     * 
     * @param serial_p Designated serial output
     */
    void set_serial_output(SerialOutput *serial_p);

    /**
     * @brief Starts the related thread.
     * 
     */
    void start_thread();
};

#endif 