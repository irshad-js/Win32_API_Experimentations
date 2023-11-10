#ifndef WIN32_EXP_
#define WIN32_EXP_

#define BSP_TICKS_PER_SEC    100U
#define BSP_SCREEN_WIDTH     96U
#define BSP_SCREEN_HEIGHT    16U

void BSP_init(void);
void BSP_sleep(uint32_t ticks);
void BSP_terminate(int result);
void BSP_DisplayNumber(int count);
void BSP_LED_Activity(int activity);
void BSP_Set_TIMER(int milliseconds);

/* special adaptations for QWIN GUI applications */
#ifdef DUAL_TARGET
    /* replace main() with main_gui() as the entry point to a GUI app. */
#define main() main_gui()
int main_gui(); /* prototype of the GUI application entry point */
#endif

#endif