#include "stm32f407xx.h"
#include "stm32f4xx_hal.h"
#include <string.h>

void led_init(void)
{
    
  __HAL_RCC_GPIOD_CLK_ENABLE();

  GPIO_InitTypeDef gpio_led;
  memset(&gpio_led, 0, sizeof(gpio_led));

  gpio_led.Pin = GPIO_PIN_13;
  gpio_led.Mode = GPIO_MODE_OUTPUT_PP;
  gpio_led.Pull = GPIO_NOPULL;
  gpio_led.Speed = GPIO_SPEED_FREQ_LOW;

  HAL_GPIO_Init(GPIOD, &gpio_led);
}

void blink_led(void)
{
    HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13);
    HAL_Delay(1000);
}
