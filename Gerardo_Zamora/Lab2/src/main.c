/*
===============================================================================
 Name        : main.c
 Author      : Gerardo Zamora
 Version     : 1.0
 Copyright   : Aww Yiss
 Description : ECE471 - Lab 2
===============================================================================
*/

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>
#include <NXP/crp.h>

// Variable to store CRP value in. Will be placed automatically
// by the linker when "Enable Code Read Protect" selected.
// See crp.h header for more information
__CRP const unsigned int CRP_WORD = CRP_NO_CRP ;

// #define's
#define LED0	(1<<19) // Define the 19th bit as LED0
#define LED1	(1<<20) // Define the 20th bit as LED1
#define LED2	(1<<21) // Define the 21th bit as LED2

void EINT3_IRQHandler(void)
{
	int a = 0;
	LPC_GPIO1 -> FIOPIN = ((~LED1) | (LED1)); // turn LEDs off
	while(1)
	{
		LPC_GPIO1 -> FIOPIN ^= LED2; // toggle LED2
		for(a = 0; a < 2000000; a++); // delay
	}
	// clear interrupt flag
	// might be needed later
	// LPC_GPIOINT -> IO0IntClr |= (1<<3);
}

void EINT0_IRQHandler(void)
{
	int a = 0;
	LPC_GPIO1 -> FIOPIN = ((~LED0) | (LED0)); // turn LEDs off
	while(1)
	{
		LPC_GPIO1 -> FIOPIN ^= LED1; // toggle LED1
		for(a = 0; a < 2000000; a++); // delay
	}
	// clear pending bit from software interrupt
	// might be needed later
	// NVIC_ClearPendingIRQ(EINT0_IRQn);
}
int main(void)
{
	// For counting to toggle LED
	static int i = 0;
	
	// Enable External Hardware Interrupt
	LPC_GPIOINT -> IO0IntEnR |= (1<<3);
	NVIC_EnableIRQ(EINT3_IRQn);

	// PriorityGroup = 5; PreemptPriority = 0; SubPriority = 0;
	// PreemptPriority = 0 implies EINT3_IRQn can interrupt other
	// interrupts with PreemptPriority > 0;
	NVIC_SetPriority(EINT3_IRQn, NVIC_EncodePriority(5,0,0));

	// Enable Software Interrupt
	NVIC_EnableIRQ(EINT0_IRQn);

	// PriorityGroup = 5; PreemptPriority = 1; SubPriority = 0;
	NVIC_SetPriority(EINT0_IRQn, NVIC_EncodePriority(5,1,0));

	LPC_GPIO1 -> FIODIR |= LED0; // Define LED0 as output
	LPC_GPIO1 -> FIODIR |= LED1; // Define LED1 as output
	LPC_GPIO1 -> FIODIR |= LED2; // Define LED2 as output

	LPC_GPIO1 -> FIOPIN = ~LED0; // Turn on LED0, turn off other LEDs
	while(1)
	{
		if(((~(LPC_GPIO0 -> FIOPIN)) & (1<<2)) == (1<<2)) // Was P0.2 pushed?
		{
			NVIC_SetPendingIRQ(EINT0_IRQn); // Set pending bit to trigger software interrupt
		}

		LPC_GPIO1 -> FIOPIN ^= LED0;	// Toggle LED0
		for (i = 0; i < 1000000; i++)	// Wait
		{
			if(((~(LPC_GPIO0 -> FIOPIN)) & (1<<2)) == (1<<2)) // Was P0.2 pushed?
			{
				NVIC_SetPendingIRQ(EINT0_IRQn); // Set pending bit to trigger software interrupt
			}
		}
	}
	return 0;
}
