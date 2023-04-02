#include "beep.h"

void Beep_Init(void)
{
	gpio_init(BEEP_PIN,GPO,0,GPIO_PIN_CONFIG);
}