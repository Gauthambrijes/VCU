/*
 * comm_driver.h
 *
 *  Created on: Jun 3, 2025
 *      Author: User
 */
#include "main.h"
#include <config.h>
#include <string.h>
#ifndef INC_COMM_DRIVER_H_
#define INC_COMM_DRIVER_H_


enum I2C_opcodes{
	APPS           = 0x01,
	BPS            = 0x02,
	Steering_pos   = 0x03,
	Wheel_speed_1  = 0x04,
	Wheel_speed_2  = 0x05,
	Wheel_speed_3  = 0x06,
	Wheel_speed_4  = 0x07,
	Coolant_temp_1 = 0x08,
	Coolant_temp_2 = 0x09,
	Damper_pot_1   = 0x0A,
	Damper_pot_2   = 0x0B,
	Damper_pot_3   = 0x0C,
	Damper_pot_4   = 0x0D,
	MAF            = 0x0E,
	LVB_Soc        = 0x0F,
	HVB_Soc        = 0x10,
	HVB_Voltage    = 0x11,
	HVB_Temp       = 0x12,
	MC_Current     = 0x13
};
//Extern Variables


//Define functions


HAL_StatusTypeDef Transmit_SensorVals(uint8_t val_type, float value);
HAL_StatusTypeDef Transmit_ErrorCode(uint16_t value);
HAL_StatusTypeDef Process_SPI_Queue(void);
#endif /* INC_COMM_DRIVER_H_ */
