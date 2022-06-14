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

#include <mbed.h>

#include "../globals/variables.h"

class PwmGenerator
{
private:
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
     * @brief PWM generator thread.
     * 
     */
    static void pwm_generator_thread();
};


