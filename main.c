#if 0
#include <stdint.h>
#include "BSP.h"

static int lcd_counter = 1111;
static int led_counter = 0;
/*..........................................................................*/
int main() {

    BSP_init(); /* initialize the Board Support Package */

    for (;;) { /* for-ever */
        BSP_sleep(100); 
        lcd_counter++;
        if ((lcd_counter % 10) == 0) {
            led_counter++;
            BSP_LED_Activity(led_counter % 4);
        }
        BSP_DisplayNumber(lcd_counter);
    }
    //return 0;
}

#endif