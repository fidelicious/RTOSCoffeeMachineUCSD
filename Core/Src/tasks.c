/*
 * tasks.c
 * Define RTOS tasks and related logic.
 *  Created on: Dec 11, 2024
 *      Author: Fidelicious
 */
#include "tasks.h"

/* -------------------- Tasks --------------------*/

/**
  * @brief  Default Task Function
  * 		Takes care of initial prompt & Reseting LEDs
  * @param  argument: Not used
  */
void StartDefaultTask(void const * argument)
{
	startupPrompt();						// User Message
	osDelay(100);
	ledCtrl(STR_LED, LED_OFF);				// Reset StrongBrew LED (Yellow)
	ledCtrl(PWR_LED, LED_OFF);				// Reset Power LED (Green)
	osDelay(100);
  /* Infinite loop */
  for(;;)
  {
	  osDelay(1);
  }
}

/**
* @brief 	Power Task Function
* 			Handles functionalities specific to when Power turns ON/OFF
* @param 	argument: Not used
*/
void powerOnTask(void const * argument)
{
  for(;;)
  {
	  // Wait for Semaphore indefinitely (till user hits power button)
	  osSemaphoreWait(powerOnSemHandle, osWaitForever);

	  if(pwr_status == false)					// Turn PWR ON
	  {
		  pwr_status = true;								// Set Power variable
		  ledCtrl(PWR_LED, LED_ON);							// Set Power LED
		  osDelay(100);
		  powerMessage();									// User Message
		  waterLevel = FULL_WATER_TANK;						// Refill water on boot
		  waterLevelMessage(waterLevel);					// User Message
		  osTimerStart(autoOffTimerHandle, AUTO_OFF_TMR);	// Start Auto-Off Timer
	  }
	  else if(pwr_status == true)				// Turn PWR OFF
	  {
		  osTimerStop(autoOffTimerHandle);					// Stop Timer in case
		  pwr_status = false;								// Set Power variable
		  ledCtrl(PWR_LED, LED_OFF);						// Reset Power LED
		  osDelay(100);
		  powerMessage();									// User Message
	  }
	  osDelay(300);
  }
}

/**
* @brief 	Brew Strength Task Function
* 			Handles functionalities specific to Enabling/Disabling a Strong Brew
* @param argument: Not used
*/
void strongBrewTask(void const * argument)
{
  for(;;)
  {
	  if(pwr_status == true)							// Only update when Power ON
	  {
		  // Wait for Semaphore indefinitely (till user hits StrongBrew button)
		  osSemaphoreWait(strongBrewSemHandle, osWaitForever);

		  ledCtrl(STR_LED, LED_TOGGLE);					// Toggle StrongBrew LED
		  osDelay(100);
		  strongBrewMessage();							// User Message
		  strongBrewCoffee = !strongBrewCoffee;			// Toggle Brew Strength
	  }
	  osDelay(300);
  }
}

/**
* @brief 	Brewing Operation Task Function
* 			Handles functionalities specific to user selecting a Size to brew
* @param 	argument: Not used
*/
void brewerTask(void const * argument)
{
	for(;;)
	{
		if(pwr_status == true)									// Brewing enabled when Power ON
		{
			uint8_t eventGroupPick = 0;

			// Wait for ISR (12, 10, 8, or 6 ozBits)
			eventGroupPick = xEventGroupWaitBits(xEventGroup,
												 travelMug_ISR_BIT | largeMug_ISR_BIT | smallMug_ISR_BIT | smallCup_ISR_BIT,
												 pdTRUE, 									// Clear on exit
												 pdFALSE, 									// don't wait on others
												 portMAX_DELAY);

			if (eventGroupPick & travelMug_ISR_BIT)				// Bitwise AND. Check if bits equal
			{
				if(enoughWaterCheck(travelMugSize))				// Check if there's enough water
				{
					brewingHelper(TVL_LED, travelMugSize);		// Run Travel Size Brew
				}
			}
			else if (eventGroupPick & largeMug_ISR_BIT)			// Bitwise AND. Check if bits equal
			{
				if(enoughWaterCheck(largeMugSize))				// Check if there's enough water
				{
					brewingHelper(LRG_LED, largeMugSize);		// Run Large Mug Size Brew
				}
			}
			else if (eventGroupPick & smallMug_ISR_BIT)			// Bitwise AND. Check if bits equal
			{
				if(enoughWaterCheck(smallMugSize))				// Check if there's enough water
				{
					brewingHelper(SMM_LED, smallMugSize);		// Run Small Mug Size Brew
				}
			}
			else if (eventGroupPick & smallCup_ISR_BIT)			// Bitwise AND. Check if bits equal
			{
				if(enoughWaterCheck(smallCupSize))				// Check if there's enough water
				{
					brewingHelper(SMC_LED, smallCupSize);		// Run Small Cup Size Brew
				}
			}
		  osDelay(300);
	  }
  }
}

/* -------------------- One-Shot Timers --------------------*/

/**
 * One shot timer for the Auto Power Shut off functionality
 * */
void pvAutoOffTimerOneShot(void const * argument)
{
	pwr_status = false;							// Power OFF Coffee Machine
	ledCtrl(PWR_LED, LED_OFF);					// Reset Power LED
	osDelay(100);								// Delay
	autoOffTimerMessage();						// User Message
}

/* -------------------- Helper Functions --------------------*/
void brewingHelper(uint8_t led, uint8_t mugSize)
{
	ledCtrl(led, LED_ON);							// Set LED
	osDelay(100);
	brewTypeMessage(mugSize);						// User Message
	progressAnimation(mugSize);						// Progress Bar animation
	osDelay(100);
	waterLevel = waterLevel - mugSize;				// Decrement Water level
	waterLevelMessage(waterLevel);					// User Message
	continueBrew = false;							// Reset continueBrew
	osTimerStart(autoOffTimerHandle, AUTO_OFF_TMR);	// Reset Auto-Off Timer
	ledCtrl(led, LED_OFF);							// Reset LED
	osDelay(100);
}


/**
 * What function does
 * @param param		usage of param
 * */
