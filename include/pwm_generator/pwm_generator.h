/**
 * @file pwm_generator.h
 * @author Halit Cetin (halit.cetin@alten.com)
 * @brief This file includes PWM Generator class headers.
 * @version 0.1
 * @date 2022-06-14
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef PWM_GENERATOR_H
#define PWM_GENERATOR_H

#include <mbed.h>

#include "../globals/enums.h"
#include "../globals/variables.h"

class PwmGenerator
{
private:
    Thread thread;
public: 
    /**
     * @brief Construct a new Pwm Generator object
     * 
     */
    PwmGenerator();

    /**
     * @brief Destroy the Pwm Generator object
     * 
     */
    ~PwmGenerator();

    /**
     * @brief Starts the related thread.
     * 
     */
    void start_thread();

    /**
     * @brief PWM generator thread.
     * 
     * @details This thread is responsible from generating sample RPM signals
     * as PWM. It is important that we are not changing the duty cycle. We are 
     * changing the period of the PWM signal.
     */
    static void pwm_generator_thread();
};

#endif 