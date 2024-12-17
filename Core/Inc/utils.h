/*
 *  utils.h
 *
 *  Created on: Dec 6, 2024
 *      Author: Fidelicious
 */
#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "main.h" // Include if HAL or global variables like huart1 are defined here
#include "globals.h"

// Function Declarations
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart);
void creationFailMessage(char* taskName);
void startupPrompt(void);
void powerMessage(void);
void autoOffTimerMessage(void);
void strongBrewMessage(void);
void brewTypeMessage(uint8_t* sizeBrew);
void waterLevelMessage(uint8_t* waterLevel);
void updateProgressBar(uint8_t progress, uint8_t oz);
void progressAnimation(uint8_t size);
bool enoughWaterCheck(uint8_t coffeeSize);
void printIT(void* value, ValueType type);
void ledCtrl(uint8_t led, uint8_t mode);
void getLedGpio(uint8_t led, GPIO_TypeDef **port, uint16_t *pin);


#endif // UTILS_H
