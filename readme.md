# STM32F407 BMP280 FreeRTOS

STM32 is communicating with BMP280 using I2C, and with PC via UART.
Two tasks are running & they are synchronized with mutex. First task reads sensor data and sends measured temperature to UART, second one does the same for pressure.
Data is represented graphically with [graph.py](./graph.py)

![Alt text](./screenshot.png?raw=true "Title")
