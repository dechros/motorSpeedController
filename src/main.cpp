#include <mbed.h>
#include <string>

enum LED_DIRECTION
{
    ERROR_DIR,
    RISING_DIR,
    FALLING_DIR
};

LED_DIRECTION led_Dir = RISING_DIR;
int led_period_ms = 0;

PwmOut led(LED1);
BufferedSerial serial(USBTX, USBRX);

int main()
{
    while (1)
    {
        if (led_Dir == RISING_DIR)
        {
            if (led_period_ms < 1100)
            {
                led_period_ms += 100;
            }
            else
            {
                led_Dir = FALLING_DIR;
            }
        }
        else if (led_Dir == FALLING_DIR)
        {
            if (led_period_ms > 100)
            {
                led_period_ms -= 100;
            }
            else
            {
                led_Dir = RISING_DIR;
            }
        }
        else
        {
            led_Dir = ERROR_DIR;
            char *message = "  ## Led direction error.";
            serial.write(message, 25);
        }

        led.period_ms(led_period_ms);
        led.write(0.50f);
        ThisThread::sleep_for(led_period_ms);
    }
}