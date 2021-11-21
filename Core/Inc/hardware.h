#ifndef CLION_STM32103_LED_CORE_INC_HARDWARE_H_
#define CLION_STM32103_LED_CORE_INC_HARDWARE_H_
#include <stdbool.h>

void hardware_init(void);

void led0_on(void);
void led0_off(void);
void led0_toggle(void);

void beep_on(void);
void beep_off(void);
void beep_toggle(void);

bool key0_is_press(void);
bool key1_is_press(void);

#endif //CLION_STM32103_LED_CORE_INC_HARDWARE_H_
