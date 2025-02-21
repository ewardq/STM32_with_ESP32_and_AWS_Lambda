/*
 * timebase.c
 *
 *  Created on: Jan 24, 2025
 *      Author: Katja
 */
#include "timebase.h"
#include "stm32f4xx.h"

#define CTRL_ENABLE      (1U<<0)       // Bit ENABLE SysTick from register CTRL
#define CTRL_TICKINT     (1U<<1)       // Bit TICKINT from register CTRL
#define CTRL_CLCKSRC     (1U<<2)       // Internal Clock source
#define CTRL_COUNTFLAG   (1U<<16)      // Flag from SysTick

#define ONE_SEC_LOAD  16000000
#define ONE_MSEC_LOAD  16000

#define MAX_DELAY        0xFFFFFFFF

#define TICK_FREQ        1             // This is the incremental step. Increment by one

volatile uint32_t g_curr_tick;
volatile uint32_t g_curr_tick_p;

uint32_t get_tick(void){
	__disable_irq();
	g_curr_tick_p = g_curr_tick;
	__enable_irq();

	return g_curr_tick_p;
}

/* Delay in seconds */
void delay(uint32_t delay){
	uint32_t tickstart = get_tick();
	uint32_t wait = delay;

	if(wait < MAX_DELAY){
		wait += (uint32_t)TICK_FREQ;
	}

	while((get_tick() - tickstart) < wait){}
}

void tick_increment(void){
	g_curr_tick += TICK_FREQ;
}

void timebase_init(void){
	/* Disable global interrupts */
	__disable_irq();

	/* Load the timer with number of clock cycles per second */
	SysTick->LOAD = ONE_MSEC_LOAD - 1;    // Subtract one because the LOAD register counts from zero.

	/* Clear SysTick current value register */
	SysTick->VAL = 0;

	/* Select internal clock source */
	SysTick->CTRL = CTRL_CLCKSRC;         // Set the register in it's entirety to 1

	/* Enable interrupt */
	SysTick->CTRL |= CTRL_TICKINT;

	/* Enable SysTick */
	SysTick->CTRL |= CTRL_ENABLE;

	/* Enable global interrupts */
	__enable_irq();
}

void SysTick_Handler(void){
	tick_increment();            // Each time this function is called, the system knows that
	                             // one second has passed
}
