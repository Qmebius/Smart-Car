#include "adc.h"

uint16  ad_value1;

void My_ADCInit(void)
{
	adc_init(POWER_ADC1_MOD,POWER_ADC1_PIN,ADC_8BIT);
}