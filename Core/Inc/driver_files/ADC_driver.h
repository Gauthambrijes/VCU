/*
 * ADC_driver.h
 *
 *  Created on: Jun 3, 2025
 *      Author: User
 */
#include "main.h"
#ifndef INC_ADC_DRIVER_H_
#define INC_ADC_DRIVER_H_


void ADC_Start();
void Convert_ADC_Values(void);
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc);

#endif /* INC_ADC_DRIVER_H_ */
