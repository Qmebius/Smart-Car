#include "electromagnet.h"

void Electromagnet_Init(void)
{
	gpio_init(Electromagnet1, GPO, 1, GPIO_PIN_CONFIG);	
	gpio_init(B13, GPO, 0, GPIO_PIN_CONFIG);
}


