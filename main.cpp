#include "mbed.h"

#if COMPONENT_RHE6616TP01_LCD
#include "lcd_api.h"
static void demo_lcd_input();
#endif

DigitalOut led1(LED1);

int main()
{
    int i = 0;

    printf("Start program ...\r\n");
#if COMPONENT_RHE6616TP01_LCD
    printf("Enable LCD program ...\r\n");
    lcd_init();
    demo_lcd_input();
#endif

    while (true) {
        printf("blinking: %d\r\n", i++);
        led1 = !led1;
        ThisThread::sleep_for(500);
#if COMPONENT_RHE6616TP01_LCD
        lcd_printNumber(i);
#endif
    }
}

#if COMPONENT_RHE6616TP01_LCD

static void demo_lcd_input()
{
    #define LCD_ALPHABET_NUM    7
    uint32_t idx = 0;
    char input, text[LCD_ALPHABET_NUM+1] = "";

    for(idx=0; idx < LCD_ALPHABET_NUM; idx++ ) {
        memset((void *)text,' ', LCD_ALPHABET_NUM);
        text[idx] = 'G';
        text[idx+1] = 'o';
        /* Set specified text on LCD */
        lcd_printf(text);
        ThisThread::sleep_for(500);
    }

    while(1)
    {
        idx = 0;
        strcpy(text, "");   // clear buffer
        printf("Input text: ");
        while(1)
        {

            input = getchar();
            printf("%c", input);

            if((input == 0xD) || (input == 0xA)) // "ENTER" key or new line to exit current input
                break;
            if( input == 0x1B) {  // Exit input demo
                return;
            }
            //strcat(text, &input);
            text[idx] = input;
            idx++;
            if(idx >= LCD_ALPHABET_NUM)
                break;
        }

        printf("\n");
        printf("Show: %s\n\n", text);

        /* Set specified text on LCD */
        lcd_printf(text);
    }   
}    
#endif