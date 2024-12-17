/*
 * comm_handler.h
 *
 *  Created on: Dec 11, 2024
 *      Author: Fidelicious
 */
#include <utils.h>
#include "cmsis_os.h"
#include "globals.h"


#ifndef COMM_HANDLER_H_
#define COMM_HANDLER_H_

// Function Declarations
void commHandlerTask(void const * argument);
void process_command(uint8_t *buffer, uint8_t length);


#endif /* COMM_HANDLER_H_ */
