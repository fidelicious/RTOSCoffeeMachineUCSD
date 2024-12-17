/*
 * utils.c
 * Utilities & Helper functions
 *
 *  Created on: Dec 6, 2024
 *      Author: Fidelicious
 */
#include "utils.h"

/* -------------------- UART Communication --------------------*/

/**
 * Transmit complete callback function
 * @param *huart		Pointer to huart for indicating when Transmission is complete
 * */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1)
    {
        uart_tx_complete = 1; // Transmission complete
    }
}

/**
 * Startup Prompt Message
 * */
void startupPrompt(void)
{
    snprintf(uart_buffer, sizeof(uart_buffer),
             "\n\n\n\n****************************************\n"
             "*           UCSD Spring 2024           *\n"
             "*            Embedded RTOS             *\n"
             "*        Fidel Quezada Guzman          *\n"
             "****************************************\n\n"
             "\tFidelicious Coffee Maker\n\n"
             "Press the ON Button to Start the Revival Process\n");

    uart_tx_complete = 0;
    if (HAL_UART_Transmit_IT(&huart1, (uint8_t *)uart_buffer, strlen(uart_buffer)) != HAL_OK)
    {
        snprintf(uart_buffer, sizeof(uart_buffer), "ERROR TRANSMITTING!");
        HAL_UART_Transmit_IT(&huart1, (uint8_t *)uart_buffer, strlen(uart_buffer));
    }
}

/**
 * Task/Resource creation failure Message
 * */
void creationFailMessage(char* taskName)
{
	char message[20] = "Failed to create: ";

	printIT(message, CHAR_TYPE);
	printIT((void*)&taskName, CHAR_TYPE);
}

/**
 * Coffee Machine On/Off Message
 * */
void powerMessage(void)
{
	char messageOn[20] = "Coffee Maker ON\n";
	char messageOff[20] = "Coffee Maker OFF\n";

    if (pwr_status == true)
    {
        printIT(messageOn, CHAR_TYPE);
    }
    else
    {
        printIT(messageOff, CHAR_TYPE);
    }
}

/**
 * Auto Off Message
 * */
void autoOffTimerMessage()
{
	char message[35] = "Auto Turning OFF Coffee Machine\n";
	printIT(message, CHAR_TYPE);
}

/**
 * Strong/Normal Brew Message
 * */
void strongBrewMessage()
{
	// print Messages
	char message1[20] = "Strong Brew\n";
	char message2[20] = "Normal Brew\n";

	  if(strongBrewCoffee == false)
	  {
		  printIT(message1, CHAR_TYPE);
	  }
	  else if(strongBrewCoffee == true)
	  {
		  printIT(message2, CHAR_TYPE);
	  }
}

/**
 * Brew Type Message
 * @param sizeBrew		Size of Brew selected by user
 * */
void brewTypeMessage(uint8_t* sizeBrew)
{
	// print options
	char strong[15] = "Strong Brew: ";
	char normal[15] = "Normal Brew: ";

	if (strongBrewCoffee == true)
	{
		printIT(strong, CHAR_TYPE);
		printIT((void*)&sizeBrew, UINT8_TYPE);
	}
	else
	{
		printIT(normal, CHAR_TYPE);
		printIT((void*)&sizeBrew, UINT8_TYPE);
	}
	printIT("oz\n", CHAR_TYPE);

}

/**
 * Water Level Message
 * @param waterLevel	Water level updated value
 * */
void waterLevelMessage(uint8_t* waterLevel)
{
	// print options
	char message[15] = "Water Level: ";
	char newLine[3] = "\n";

	// print
	printIT(message, CHAR_TYPE);
	printIT((void*)&waterLevel, UINT8_TYPE);
	printIT(newLine, CHAR_TYPE);
}

/**
 * Progress Bar Animation updater
 * @param progress		Input Value to print
 * @param oz			Size in oz
 * */
void updateProgressBar(uint8_t progress, uint8_t oz)
{
    const uint8_t barWidth = 40;
    uint8_t completedWidth = barWidth * progress / oz;

    char progressBar[barWidth + 1];
    memset(progressBar, ' ', barWidth);
    progressBar[barWidth] = '\0';

    for (uint8_t i = 0; i < completedWidth; ++i)
    {
        progressBar[i] = '*';
    }

    snprintf(uart_buffer, sizeof(uart_buffer), "[%s] %d oz\r\n", progressBar, progress);

    uart_tx_complete = 0;
    if (HAL_UART_Transmit_IT(&huart1, (uint8_t *)uart_buffer, strlen(uart_buffer)) != HAL_OK)
    {
        snprintf(uart_buffer, sizeof(uart_buffer), "ERROR TRANSMITTING!\n");
        HAL_UART_Transmit_IT(&huart1, (uint8_t *)uart_buffer, strlen(uart_buffer));
    }
    HAL_Delay(20);
}

/**
 * Progress Bar Animation
 * @param size		Input Value to print
 * */
void progressAnimation(uint8_t size)
{
	for (int i = 0; i <= size; i++)
	{
		updateProgressBar(i, size);
		osDelay(50);  					// Add delay to simulate progress
	}
}

/* -------------------- Coffee Maker Functionality --------------------*/

/**
 * Check if Coffee Machine has enough water
 * @param coffeeSize	Size of coffee that will be brewed next
 * */
bool enoughWaterCheck(uint8_t coffeeSize)
{
	char message1[50] = "Water Level is too low!!\nAuto-Refilling now...\n";
	char message2[55] = "Water Level is full now!\nPlease select Coffee Again\n";

	// Check if we will have enough water after CoffeeSize selection
	if(waterLevel < coffeeSize)
	{
		// Print Message to user
		printIT(message1, CHAR_TYPE);

		// Auto-Refill Water w/ animation
		for (int i = 0; i <= 40; i++)
		{
		  updateProgressBar(i, 40);
		}
		waterLevel = FULL_WATER_TANK;

		// Print Message to user
		printIT(message2, CHAR_TYPE);

	    return false;	// wont proceed w/ brew
	}
	else
	{
		return true;	// proceed w/ brew
	}
}


/* -------------------- Helper Functions --------------------*/

/**
 * Helper Function for type based printing
 * @param value			Input Value to print
 * @param type			Data Type to handle
 * */
void printIT(void* value, ValueType type)
{
	// Use buffer based on type of value we want to print
    switch (type)
    {
        case CHAR_TYPE:
            snprintf(uart_buffer, sizeof(uart_buffer), "%s", (char*)value);
            break;
        case UINT8_TYPE:
            snprintf(uart_buffer, sizeof(uart_buffer), "%u", *(uint8_t*)value);
            break;
        default:
            snprintf(uart_buffer, sizeof(uart_buffer), "Unknown type\n");
            break;
    }

    // UART Print
    uart_tx_complete = 0;
    if (HAL_UART_Transmit_IT(&huart1, (uint8_t *)uart_buffer, strlen(uart_buffer)) != HAL_OK)
    {
        snprintf(uart_buffer, sizeof(uart_buffer), "ERROR TRANSMITTING!\n");
        HAL_UART_Transmit_IT(&huart1, (uint8_t *)uart_buffer, strlen(uart_buffer));
    }
}

/**
 * Green LED Control
 * @param led		LED to control
 * @param mode		LED Mode (On/off/toggle)
 * */
void ledCtrl(uint8_t led, uint8_t mode)
{
	// Init Port & Pin variables
	GPIO_TypeDef *port = NULL;
	uint16_t pin = 0;

	// Assign LED GPIO port & pin values based on led param
	getLedGpio(led, &port, &pin);

	// Control LED based on mode param
	switch (mode)
	{
		case LED_OFF:										// OFF
			HAL_GPIO_WritePin(port, pin,GPIO_PIN_RESET);
			break;

		case LED_ON:										// ON
			HAL_GPIO_WritePin(port, pin,GPIO_PIN_SET);
			break;

		case LED_TOGGLE:									// Toggle
			HAL_GPIO_TogglePin(port, pin);
			break;

        default:
            break;
	}
}

/**
 * Assigns the LED GPIO port & pin values by reference
 * @param led		LED in question
 * @param **port	Pointer to *port of led param
 * @param *pin		Pointer to pin of led param
 * */
void getLedGpio(uint8_t led, GPIO_TypeDef **port, uint16_t *pin)
{
    *port = NULL;
    *pin = 0;

    // Assign port & pin values by reference
    switch (led)
    {
        case PWR_LED:									// Power (Green)
            *port = LED_PWR_ON_GPIO_Port;
            *pin = LED_PWR_ON_Pin;
            break;

        case STR_LED:									// Strength (Yellow)
            *port = LED_STR_BRW_GPIO_Port;
            *pin = LED_STR_BRW_Pin;
            break;

        case TVL_LED:									// Traveler Size (Red)
            *port = LED_TRAV_MUG_GPIO_Port;
            *pin = LED_TRAV_MUG_Pin;
            break;

        case LRG_LED:									// Large Size (Red)
            *port = LED_LARG_MUG_GPIO_Port;
            *pin = LED_LARG_MUG_Pin;
            break;

        case SMM_LED:									// Small Mug Size (Red)
            *port = LED_SMALL_MUG_GPIO_Port;
            *pin = LED_SMALL_MUG_Pin;
            break;

        case SMC_LED:									// Small Cup Size (Red)
            *port = LED_SMALL_CUP_GPIO_Port;
            *pin = LED_SMALL_CUP_Pin;
            break;

        default:
            break;
    }
}




/**
 * What function does
 * @param param		usage of param
 * */
