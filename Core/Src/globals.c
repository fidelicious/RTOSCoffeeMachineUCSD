/*
 * globals.c
 * Global variables & constants stored here
 *
 *  Created on: Dec 6, 2024
 *      Author: Fidelicious
 */

#include "globals.h"

// Global UART variables
char uart_buffer[300];
volatile uint8_t uart_tx_complete = 1;

// Power and brew status
volatile bool pwr_status = false;
volatile bool continueBrew = false;
volatile bool strongBrewCoffee = false;

// System variables
uint8_t waterLevel = FULL_WATER_TANK;

// Coffee Sizes (in oz)
const uint8_t travelMugSize = 12;
const uint8_t largeMugSize = 10;
const uint8_t smallMugSize = 8;
const uint8_t smallCupSize = 6;

// Command buffer
uint8_t cmd_buffer[CMD_BUFFER_SIZE];


