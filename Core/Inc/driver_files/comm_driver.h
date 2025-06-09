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


#define SLAVE_ADDRESS 0x02
enum I2C_opcodes{
	APPS           = 0x01,
	BPS            = 0x02,
	Steering_pos   = 0x03,
	Wheel_speed    = 0x04,
	Coolant_temp_1 = 0x05,
	Coolant_temp_2 = 0x06,
	Damper_pot_1   = 0x07,
	Damper_pot_2   = 0x08,
	Damper_pot_3   = 0x09,
	Damper_pot_4   = 0x0A,
	MAF            = 0x0B,
	LVB_Soc        = 0x0C,
	HVB_Soc        = 0x0D,
	HVB_Voltage    = 0x0E,
	HVB_Temp       = 0x0F,
	MC_Current     = 0x10
};
//Extern Variables


//Define functions


HAL_StatusTypeDef Transmit_SensorVals(uint8_t val_type, float value);
HAL_StatusTypeDef Transmit_ErrorCode(uint16_t value);
HAL_StatusTypeDef Process_SPI_Queue(void);
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi);
void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi);
void SPI_CS_Enable(void);
void SPI_CS_Disable(void);


#endif /* INC_COMM_DRIVER_H_ */
