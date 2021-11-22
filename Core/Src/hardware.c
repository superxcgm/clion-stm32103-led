
#include <hardware.h>
#include <stdbool.h>
#include "stm32f1xx_hal.h"

#define LED0_Pin GPIO_PIN_5
#define LED0_GPIO_Port GPIOB

#define BEEP_Pin GPIO_PIN_8
#define BEEP_GPIO_Port GPIOB

#define KEY0_Pin GPIO_PIN_4
#define KEY0_GPIO_Port GPIOE

#define KEY1_Pin GPIO_PIN_3
#define KEY1_GPIO_Port GPIOE

#define KEY_UP_Pin GPIO_PIN_0
#define KEY_UP_GPIO_Port GPIOA

static void init_led0(void) {
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin : LED0_Pin */
  GPIO_InitStruct.Pin = LED0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED0_GPIO_Port, &GPIO_InitStruct);
}

static void init_beep(void) {
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : BEEP_Pin */
  GPIO_InitStruct.Pin = BEEP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(BEEP_GPIO_Port, &GPIO_InitStruct);
}

static void init_keys(void) {
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = KEY1_Pin | KEY0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
}

static bool key_scan_long_press(GPIO_TypeDef  *GPIOx, uint16_t GPIO_PIN) {
  if (HAL_GPIO_ReadPin(GPIOx, GPIO_PIN) == GPIO_PIN_RESET) {
    HAL_Delay(10);
    if (HAL_GPIO_ReadPin(GPIOx, GPIO_PIN) == GPIO_PIN_RESET) {
      return true;
    }
  }
  return false;
}

static bool key_scan_no_long_press(GPIO_TypeDef  *GPIOx, uint16_t GPIO_PIN) {
  static bool is_key_up = true;
  if (is_key_up && HAL_GPIO_ReadPin(GPIOx, GPIO_PIN) == GPIO_PIN_RESET) {
    HAL_Delay(10);
    is_key_up = false;
    if (HAL_GPIO_ReadPin(GPIOx, GPIO_PIN) == GPIO_PIN_RESET) {
      return true;
    }
  } else if (HAL_GPIO_ReadPin(GPIOx, GPIO_PIN) == GPIO_PIN_SET) {
    is_key_up = true;
  }
  return false;
}

void hardware_init(void) {
  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();


  init_led0();
  init_beep();
  init_keys();
  extix_init();
}


void led0_on(void) {
  HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_RESET);
}

void led0_off(void) {
  HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_SET);
}

void led0_toggle(void) {
  GPIO_PinState state = HAL_GPIO_ReadPin(LED0_GPIO_Port, LED0_Pin);
  HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, !state);
}

void beep_on(void) {
  HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_SET);
}

void beep_off(void) {
  HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_RESET);
}

void beep_toggle(void) {
  GPIO_PinState state = HAL_GPIO_ReadPin(BEEP_GPIO_Port, BEEP_Pin);
  HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, !state);
}

bool key0_is_press(void) {
  return key_scan_long_press(KEY0_GPIO_Port, KEY0_Pin);
}

bool key1_is_press(void) {
  return key_scan_no_long_press(KEY1_GPIO_Port, KEY1_Pin);
}

void extix_init(void) {
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = KEY_UP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(KEY_UP_GPIO_Port, &GPIO_InitStruct);

  HAL_NVIC_SetPriority(EXTI0_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);
}

void EXTI0_IRQHandler(void) {
  HAL_GPIO_EXTI_IRQHandler(KEY_UP_Pin);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
  // todo: 防抖，但是不能用HAL_Delay
  switch (GPIO_Pin) {
    case KEY_UP_Pin:
      led0_toggle();
//      beep_toggle();
      break;
  }
}
