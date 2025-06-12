/*
 * ADC_driver.c
 *
 *  Created on: Jun 3, 2025
 *      Author: Ajay
 */

/*



MAKE SURE TO CHANGE RANKS ACCORDING TO SENSORS



*/
#include <ADC_driver.h>

#define ADC_BUFFER_SIZE 6

extern ADC_HandleTypeDef hadc1;
extern DMA_HandleTypeDef hdma_adc1;

__attribute__((aligned(32))) struct {
    uint16_t samples[ADC_BUFFER_SIZE];  // Actual data
    uint16_t _reserved[16 - ADC_BUFFER_SIZE]; // Padding to 32 bytes, These are just fillers like your life
} ADC_raw;

float ADC_V[ADC_BUFFER_SIZE];


void ADC_Start(){
	memset(&ADC_raw, 0, sizeof(ADC_raw));
	memset(&ADC_V, 0, sizeof(ADC_V));
	HAL_ADC_Start_DMA(&hadc1,(uint32_t *)&ADC_raw, ADC_BUFFER_SIZE);
}

void Convert_ADC_Values(void) {
	const float VREF = 3.3f;
	const float VRES = 65535.0f;
	const float VCNF = VREF/VRES;
    for (int i = 0; i < ADC_BUFFER_SIZE; i++) {
        ADC_V[i] = (float)ADC_raw.samples[i] * VCNF ;
    }
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
	SCB_InvalidateDCache_by_Addr((uint16_t*)&ADC_raw, sizeof(ADC_raw));
	//Conversion
	Convert_ADC_Values();
	//Call the function that converts and returns Voltage to sensor values

}
