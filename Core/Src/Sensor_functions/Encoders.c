/*
 * Encoders.c
 *
 *  Created on: Jun 3, 2025
 *      Author: User
 */

#include <Encoders.h>



#define Gears 60



float Wheel_speed_val_1 = 0.0f;
float Wheel_speed_val_2 = 0.0f;

void Start_WheelSpeed(){
	HAL_TIM_Base_Start_IT(&htim6);
	HAL_TIM_Base_Start(&htim3);
	HAL_TIM_Base_Start(&htim4);
}

// Callback function (executes every 1 second)
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
  if (htim->Instance == TIM6) {
    //encoder function
	  uint32_t Wheelspeed_Count_1 = __HAL_TIM_GET_COUNTER(&htim3);
	  __HAL_TIM_SET_COUNTER(&htim3, 0);
	  uint32_t Wheelspeed_Count_2 = __HAL_TIM_GET_COUNTER(&htim4);
	  __HAL_TIM_SET_COUNTER(&htim4, 0);
	  RPM(Wheelspeed_Count_1,Wheelspeed_Count_2);
  }
  else if (htim->Instance == TIM7)  {
	  HAL_TIM_Base_Stop_IT(&htim7);
	  __HAL_TIM_SET_COUNTER(&htim7, 0);
	  APPS_SD_Check();
  }
}


#ifdef FECU
void RPM(uint32_t Wheelspeed_Count_1,uint32_t Wheelspeed_Count_2){
	Wheel_speed_val_1 = (float)(Wheelspeed_Count_1/Gears);
	Wheel_speed_val_2 = (float)(Wheelspeed_Count_2/Gears);
	Transmit_SensorVals(Wheel_speed_1, Wheel_speed_val_1);
	Transmit_SensorVals(Wheel_speed_2, Wheel_speed_val_2);

}

#elif RECU
void RPM(){
	Wheel_speed_val_1 = (float)(Wheelspeed_Count_1/Gears);
	Wheel_speed_val_2 = (float)(Wheelspeed_Count_2/Gears);
	Transmit_SensorVals(Wheel_speed_3, Wheel_speed_val_1);
	Transmit_SensorVals(Wheel_speed_4, Wheel_speed_val_2);
}


#endif
