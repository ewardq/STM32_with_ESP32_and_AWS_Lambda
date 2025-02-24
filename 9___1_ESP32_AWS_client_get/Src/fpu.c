/*
 * fpu.h
 *
 *  Created on: Jan 22, 2025
 *      Author: Katja
 */
#include "stm32f4xx.h"
#include "fpu.h"

void fpu_enable(void){
	/* Enable floating point unit: Enable CP10 and CP11 full access*/
	SCB->CPACR |= (1<<20); // Set only bit 20 to ONE
	SCB->CPACR |= (1<<21); // Set only bit 21 to ONE
	SCB->CPACR |= (1<<22); // Set only bit 22 to ONE
	SCB->CPACR |= (1<<23); // Set only bit 23 to ONE
}
