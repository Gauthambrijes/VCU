/*
 * Potentiometers.c
 *
 *  Created on: Jun 3, 2025
 *      Author: User
 */

#include <Potentiometers.h>
extern TIM_HandleTypeDef htim7;
float BPS_val = 0.0f;
float Steering_pos_val = 0.0f;

float Damper_pot_1_val = 0.0f;
float Damper_pot_2_val = 0.0f;
float Damper_pot_3_val = 0.0f;
float Damper_pot_4_val = 0.0f;

uint8_t APPS_Err;

void APPS_SCS(uint16_t APPS_1,uint16_t APPS_2){
	uint16_t deviation = abs(APPS_1-APPS_2);
	uint16_t limit = 6553;
	if(deviation>limit){
		HAL_TIM_Base_Start_IT(&htim7);
		APPS_Err = 1;
	}
	else{
		APPS_Err = 0;
	}
	//TO MC THRU CAN
}

void APPS_SD_Check(){
	if(APPS_Err==1){
		Error_Handler();
	}
}
