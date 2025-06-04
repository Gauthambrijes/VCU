/*
 * comm_driver.h
 *
 *  Created on: Jun 3, 2025
 *      Author: User
 */
#include "main.h"
#ifndef INC_COMM_DRIVER_H_
#define INC_COMM_DRIVER_H_

#define SLAVE_ADDRESS 0x02
enum sensor_opcodes{
	APPS         = 0x01,
	BPS          = 0x02,
	Steering_pos = 0x03,
	Wheel_speed  = 0x04,
	Coolant_temp = 0x05,
	Damper_pot   = 0x06,


};

#endif /* INC_COMM_DRIVER_H_ */
