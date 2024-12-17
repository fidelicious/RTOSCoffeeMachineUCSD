/*
 * tasks.h
 *
 *  Created on: Dec 11, 2024
 *      Author: Fidelicious
 */

#ifndef TASKS_H_
#define TASKS_H_

#include "main.h"
#include "cmsis_os.h"
#include "globals.h"
#include "utils.h"


// Function Declarations
void StartDefaultTask(void const * argument);
void powerOnTask(void const * argument);
void strongBrewTask(void const * argument);
void brewerTask(void const * argument);
void pvAutoOffTimerOneShot(void const * argument);
void brewingHelper(uint8_t led, uint8_t mugSize);
#endif /* TASKS_H_ */
