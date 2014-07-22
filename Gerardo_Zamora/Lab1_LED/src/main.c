/*
===============================================================================
 Name        : main.c
 Author      : Gerardo Zamora
 Version     : 1.0
 Copyright   : No copyright xD
 Description : main definition
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
#define LED3	(1<<22) // Define the 22th bit as LED3
#define LED4	(1<<23) // Define the 23th bit as LED4
#define LED5	(1<<24) // Define the 24th bit as LED5
#define LED6	(1<<25) // Define the 25th bit as LED6
#define LED7	(1<<26) // Define the 26th bit as LED7

// Global variables
volatile int off_on = 1; // turn off or on the LEDs
int currentLED = LED0; // Holds state of LEDs

void EINT3_IRQHandler(void)
{
	int a = 0;
	LPC_GPIO1 -> FIOPIN = ((~currentLED) | (currentLED)); // turn LEDs off
	off_on = !off_on; // toggle
	for(a = 0; a < 2000000; a++); // delay
	LPC_GPIOINT -> IO0IntClr |= (1<<2); // clear interrupt flag
}

int main(void)
{
	LPC_GPIOINT -> IO0IntEnR |= (1<<2);
	NVIC_EnableIRQ(EINT3_IRQn);
	volatile int i = 0; // Counting variable for time
	volatile int j = 0; // Counting variable for cyclone
	volatile int k = 0; // Counting variable for fading
	volatile int x = 0; // Counting variable to implement fading
	LPC_GPIO1 -> FIODIR |= LED0; // Define LED0 as output
	LPC_GPIO1 -> FIODIR |= LED1; // Define LED1 as output
	LPC_GPIO1 -> FIODIR |= LED2; // Define LED2 as output
	LPC_GPIO1 -> FIODIR |= LED3; // Define LED3 as output
	LPC_GPIO1 -> FIODIR |= LED4; // Define LED4 as output
	LPC_GPIO1 -> FIODIR |= LED5; // Define LED5 as output
	LPC_GPIO1 -> FIODIR |= LED6; // Define LED6 as output
	LPC_GPIO1 -> FIODIR |= LED7; // Define LED7 as output

	while (1)
	{
		if (off_on)
		{
			for (i = 0; i < 7; i++) // LED0 to LED7
			{
				// Fading LED
				for (k = 0; k < 1000; k++)
				{
					LPC_GPIO1 -> FIOPIN = ~currentLED; 				// Turn on current LED
					for (j = 0; j < 8000-x; j++){while(!off_on);}; 	// wait with LED on
					LPC_GPIO1 -> FIOPIN |= currentLED;			 	// Turn off current LED
					for (j = 0; j < x; j++){while(!off_on);};		// wait with LED off
					x += 10;										// Increase fading counting variable
				}
				currentLED <<= 1; // get next LED
				x = 0;
			}

			for (i = 0; i < 7; i++) // LED7 to LED0
			{
				// Fading LED
				for (k = 0; k < 1000; k++)
				{
					LPC_GPIO1 -> FIOPIN = ~currentLED; 				// Turn on current LED
					for (j = 0; j < 8000-x; j++){while(!off_on);};	// wait with LED on
					LPC_GPIO1 -> FIOPIN |= currentLED;				// Turn off current LED
					for (j = 0; j < x; j++){while(!off_on);};		// wait with LED off
					x += 10;										// Increase fading counting variable
				}
				x = 0;
				currentLED >>= 1; // get next LED
			}
		}
		else;
	}

	return 0;
}
