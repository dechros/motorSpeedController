/**
 * @file main.cpp
 * @author Halit Cetin (halit.cetin@alten.com)
 * @brief Program entry file.
 * @version 0.1
 * @date 2022-06-14
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <mbed.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>

#include "enums.h"
#include "variables.h"
#include "serial_output.h"
#include "pwm_generator.h"
#include "pwm_capturing.h"
#include "data_manager.h"
#include "sd_manager.h"

int main()
{
    serial_set_pins(USBTX, USBRX);
    set_sd_card(PB_5, PB_4, PB_3, PA_4);
    set_time_date();
    serial_set_pins_esp_sd(D1, D0);
    pwm_generator_set_pins(LED1, D9);
    pwm_generator_start_thread();
    pwm_capturing_set_pin(D8);
    pwm_capturing_start_thread();
    set_ethernet_interface();
    data_manager_start_thread();
    ThisThread::sleep_for(osWaitForever);
}