/*
 * ApplicationApi.c
 * Functionality accessible for Firmware API Software
 *
 *  Created on: Dec 13, 2024
 *      Author: Fidelicious
 */
#include "ApplicationApi.h"


/**
 * Toggle Coffee Maker Main Power
 * */
void togglePower(void)
{
	pwr_status = !pwr_status;
}

/**
 * Gets status of Coffee Maker Main Power
 * */
bool getPowerStatus(void)
{
	return pwr_status;
}

/**
 * What function does
 * @param param		usage of param
 * */
