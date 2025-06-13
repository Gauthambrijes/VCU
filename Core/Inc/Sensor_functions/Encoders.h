/*
 * Encoders.h
 *
 *  Created on: Jun 3, 2025
 *      Author: User
 */
#include "main.h"

#ifndef INC_SENSOR_FUNCTIONS_ENCODERS_H_
#define INC_SENSOR_FUNCTIONS_ENCODERS_H_
extern I2C_opcodes;
extern TIM_HandleTypeDef htim6;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim7;
void RPM();
void Start_WheelSpeed();
#endif /* INC_SENSOR_FUNCTIONS_ENCODERS_H_ */
