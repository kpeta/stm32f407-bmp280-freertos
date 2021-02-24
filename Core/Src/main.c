#include "stm32f407xx.h"
#include "stm32f4xx_hal.h"
#include <string.h>
#include <stdio.h>
#include "led.h"
#include "uart.h"
#include "i2c.h"
#include "bmp280.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

void task_temp(void *pvParameters);
void task_press(void *pvParameters);
xSemaphoreHandle mutex;

BMP280_HandleTypedef bmp280;

uint32_t pressure;
int32_t temperature;
uint16_t size;
uint8_t Data[256];

int main(void)
{
	HAL_Init();
	
	led_init();
	uart_init();
	i2c_init();
	bmp280_init();

	mutex = xSemaphoreCreateMutex();

	xTaskCreate(&task_temp, "temperature", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
	xTaskCreate(&task_press, "pressure", configMINIMAL_STACK_SIZE, NULL, 1, NULL);

	vTaskStartScheduler();

	while(1);

}

static uint32_t i1, i2;

void task_temp(void *pvParameters)
{
	while (1)
	{
		if (xSemaphoreTake(mutex, portMAX_DELAY))
		{
			i1++;
			bmp280_read_fixed(&bmp280, &temperature, &pressure);
			size = sprintf((char *)Data,"%ld,%ld.%ld,t\r\n", i1, temperature/100, temperature % 100);
			HAL_UART_Transmit(&huart2, Data, size, 1000);
			//HAL_UART_Transmit(&huart2, (uint8_t *) "task1\n", strlen("task1\n"), 1000);
			xSemaphoreGive(mutex);
			vTaskDelay (500 / portTICK_RATE_MS );
		}

	}

}

void task_press(void *pvParameters)
{
 	while (1)
	{
		if (xSemaphoreTake(mutex, portMAX_DELAY))
		{
			i2++;
			bmp280_read_fixed(&bmp280, &temperature, &pressure);
			size = sprintf((char *)Data,"%ld,%ld,p\r\n", i2, pressure/256);
			HAL_UART_Transmit(&huart2, Data, size, 1000);
			//HAL_UART_Transmit(&huart2, (uint8_t *) "task2\n", strlen("task2\n"), 1000);
			xSemaphoreGive(mutex);
			vTaskDelay (500 / portTICK_RATE_MS );
		}
	}
}


void xPortSysTickHandler (void);

void SysTick_Handler(void)
{
  HAL_IncTick();
  if(xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED) xPortSysTickHandler();
}

void HardFault_Handler()
{
	while (1);
}
