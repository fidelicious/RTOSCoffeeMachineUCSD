/*
 * comm_handler.c
 * Handle UART communication, commands, and responses
 *
 *  Created on: Dec 11, 2024
 *      Author: Fidelicious
 */
#include "comm_handler.h"

// External Variables (defined in main.c)
extern UART_HandleTypeDef huart1;
extern osSemaphoreId strongBrewSemHandle;

/**
 * Communication Handler Task
 * @param argument		arguments to handle
 * */
void commHandlerTask(void const * argument)
{
    uint8_t received_byte;
    uint8_t cmd_index = 0;

	/* Infinite loop */
    for (;;)
    {
        // Read byte-by-byte from UART
        if (HAL_UART_Receive(&huart1, &received_byte, 1, HAL_MAX_DELAY) == HAL_OK)
        {
            cmd_buffer[cmd_index++] = received_byte;

            // Check if command is complete (e.g., Length byte received)
            if (cmd_index >= 2 && cmd_index == cmd_buffer[1] + 2)
            {
                process_command(cmd_buffer, cmd_index);
                cmd_index = 0; // Reset buffer index
            }
        }
    }
}


/**
 * Command Processing
 * @param buffer		command buffer to process
 * @param length		buffer length
 * */
void process_command(uint8_t *buffer, uint8_t length)
{
	// Validate input buffer length
	if (length < 2) // Minimum required: cmd_id + payload_length
	{
		printf("Invalid command length: %d\n", length);
		return;
	}
    uint8_t cmd_id = buffer[0];
    uint8_t payload_length = buffer[1];

	// Validate payload length
	if (length != (2 + payload_length)) // Total length = cmd_id + payload_length + payload
	{
		printf("Invalid payload length: %d (expected %d)\n", length, 2 + payload_length);
		return;
	}
    uint8_t *payload = &buffer[2];

	switch (cmd_id)
	{
		case CMD_POWER_ON_OFF:
			togglePower();
			break;

		case CMD_TOGGLE_STRONG_BREW:
			osSemaphoreRelease(strongBrewSemHandle);
			break;

		case CMD_GET_PWR_STATUS:
		{
			uint8_t response = 0x00; 				// Default: OFF
			bool powerStatus = getPowerStatus();

			if (powerStatus == true)
			{
				response = 0x01; 					// ON
			}

			// Debug log
			printf("Power Status: %d, Response: 0x%02X\n", powerStatus, response);

			HAL_UART_Abort(&huart1);   // Abort ongoing transfers
			osDelay(10); // 10 ms delay to allow host to prepare for response

			// Send response via UART (blocking mode for consistency)
			if (HAL_UART_Transmit(&huart1, &response, 1, HAL_MAX_DELAY) != HAL_OK)
			{
				printf("UART Transmit Error!\n");
			}
			break;
		}

		case CMD_SET_WATER_LEVEL:
			if (payload_length >= 1)
			{
				waterLevel = payload[0];
				waterLevelMessage(waterLevel);
			}
			break;
		default:
			// Unknown Command
			break;
	}
}


/**
 * What function does
 * @param param		usage of param
 * */
