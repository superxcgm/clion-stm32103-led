
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
  GPIO_InitTypeDef GPIO_InitStruct0 = {0};
  GPIO_InitStruct0.Pin = KEY1_Pin | KEY0_Pin;
  GPIO_InitStruct0.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct0.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct0);
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
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();

  init_led0();
  init_beep();
  init_keys();
}

static bool led0_is_on = false;

void led0_on(void) {
  HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_RESET);
  led0_is_on = true;
}

void led0_off(void) {
  HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_SET);
  led0_is_on = false;
}

void led0_toggle(void) {
  if (led0_is_on) {
    led0_off();
  } else {
    led0_on();
  }
}

static bool beep_is_on = false;

void beep_on(void) {
  HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_SET);
  beep_is_on = true;
}

void beep_off(void) {
  HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_RESET);
  beep_is_on = false;
}

void beep_toggle(void) {
  if (beep_is_on) {
    beep_off();
  } else {
    beep_on();
  }
}

bool is_key0_press(void) {
  return key_scan_long_press(KEY0_GPIO_Port, KEY0_Pin);
}

bool is_key1_press(void) {
  return key_scan_no_long_press(KEY1_GPIO_Port, KEY1_Pin);
}

