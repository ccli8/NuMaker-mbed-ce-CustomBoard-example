#include "mbed.h"

DigitalOut led1(LED1);

int main()
{
    int i = 0;

    while (true) {
        printf("blinking: %d\r\n", i++);
        led1 = !led1;
        ThisThread::sleep_for(500);
    }
}
