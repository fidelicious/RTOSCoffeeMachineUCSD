/*
 * globals.h
 *
 *  Created on: Dec 6, 2024
 *      Author: Fidelicious
 */

#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdint.h>
#include <stdbool.h>
#include "main.h"
#include "cmsis_os.h"
#include "typedefs.h"


#define CMD_BUFFER_SIZE 32
#define FULL_WATER_TANK 100
#define PWR_LED 		0			// LEDs
#define STR_LED 		1
#define TVL_LED 		2
#define LRG_LED 		3
#define SMM_LED 		4
#define SMC_LED 		5
#define LED_OFF 		0			// LED States
#define LED_ON  		1
#define LED_TOGGLE 		2
#define AUTO_OFF_TMR	60000		// Timer value (ms)


// External Handle Variables
extern I2C_HandleTypeDef hi2c1;
extern UART_HandleTypeDef huart1;
extern osThreadId StartDefaultTaskHandle;
extern osThreadId powerOnTaskHandle;
extern osThreadId strongBrewTaskHandle;
extern osThreadId brewerTaskHandle;
extern osTimerId autoOffTimerHandle;
extern osMutexId brewingMutexHandle;
extern osSemaphoreId powerOnSemHandle;
extern osSemaphoreId strongBrewSemHandle;
extern EventGroupHandle_t xEventGroup;

// Global UART variables
extern char uart_buffer[300];
extern volatile uint8_t uart_tx_complete;

// Power and brew status
extern volatile bool pwr_status;
extern volatile bool continueBrew;
extern volatile bool strongBrewCoffee;

// System variables
extern uint8_t waterLevel;

// Coffee Sizes (in oz)
extern const uint8_t travelMugSize;
extern const uint8_t largeMugSize;
extern const uint8_t smallMugSize;
extern const uint8_t smallCupSize;

// Command buffer
extern uint8_t cmd_buffer[CMD_BUFFER_SIZE];

#endif // GLOBALS_H

