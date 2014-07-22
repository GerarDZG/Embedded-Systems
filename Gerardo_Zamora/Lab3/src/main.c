/*
===============================================================================
 Name        : main.c
 Author      : Gerardo Zamora
 Version     : 1.0
 Copyright   : Aww Yiss
 Description : Stopwatch?
===============================================================================
*/

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>
#include <NXP/crp.h>
#include "lcd.h"
#include <stdio.h>

// Variable to store CRP value in. Will be placed automatically
// by the linker when "Enable Code Read Protect" selected.
// See crp.h header for more information
__CRP const unsigned int CRP_WORD = CRP_NO_CRP;

// #define's
#define LED0	(1<<19) // Define the 19th bit as LED0
#define LED1	(1<<20) // Define the 20th bit as LED1
#define LED2	(1<<21) // Define the 21th bit as LED2
#define LED3	(1<<22) // Define the 21th bit as LED3

// global variables
uint32_t hundred_ms = 0;
uint32_t seconds = 0;
uint32_t minutes = 0;
uint32_t hours = 0;
char nameOfLab[] = "Stopwatch by GdoZ";
char copyright[] = "Copyright: Aww Yiss";
char message[10] = " h  m  s";
char time[11];
void TIMER0_IRQHandler(void)
{

	hundred_ms++;

	if (hundred_ms >= 10)
	{
		hundred_ms = 0;
		seconds++;
	}
	if (seconds >= 60)
	{
		seconds = 0;
		minutes++;
	}
	if (minutes >= 60)
	{
		minutes = 0;
		hours++;
	}

	LPC_TIM0 -> IR |= 1;
}

void EINT3_IRQHandler(void)
{
	if(((~(LPC_GPIO0 -> FIOPIN)) & (1<<3)) == (1<<3)) // Was P0.3 pushed?
	{
		// Set values to zero
		hundred_ms = 0;
		seconds = 0;
		minutes = 0;
		hours = 0;

		sprintf(time,"%2d:%2d:%2d.%d",hours,minutes,seconds,hundred_ms);
		drawString(1,28,time);

		// Have some delay
		delay_ms(50);
	}
	else if(((~(LPC_GPIO0 -> FIOPIN)) & (1<<2)) == (1<<2)) // Was P0.2 pushed?
	{
		// Start/Stop TIMER0 if P0.2
		LPC_TIM0 -> TCR ^= 1;
		delay_ms(20);
	}

	LPC_GPIOINT -> IO0IntClr |= (1<<2) + (1<<3);
}

int main(void)
{
	// Initialize TIMER0 to 10Hz
	// MCR = 0011; BIT0 = 1 to enable TIMER0
	// and BIT1 = 1 to reset timer clock when match.
	LPC_TIM0 -> MCR = (1<<1)+(1);
	LPC_TIM0 -> MR0 = 2500000; // 10Hz timer
	LPC_TIM0 -> TCR = 1;

	// Enable TIMER0
	NVIC_EnableIRQ(TIMER0_IRQn);

	// Initialize rising edge interrupt at P0.3
	LPC_GPIOINT -> IO0IntEnR |= (1<<2) + (1<<3);

	// Enable EINT3 for the External Hardware Interrupt
	NVIC_EnableIRQ(EINT3_IRQn);

	// Set LED0 through LED3 as outputs
	LPC_GPIO1 -> FIODIR |= LED0 + LED1 + LED2 + LED3;

	// Initialize LCD values
	lcd_init();
	fillScreen(ST7735_16_WHITE);
	setBackgroundColor16(ST7735_16_WHITE);
	drawString(1,1,nameOfLab);
	drawString(1,10,copyright);
	drawString(1,19,message);

	// PriorityGroup = 5; PreemptPriority = 0; SubPriority = 0;
	// PreemptPriority = 0 implies EINT3_IRQn can interrupt other
	// interrupts with PreemptPriority > 0;
//	NVIC_SetPriority(EINT3_IRQn, NVIC_EncodePriority(5,0,0));

	// PriorityGroup = 5; PreemptPriority = 1; SubPriority = 0;
//	NVIC_SetPriority(EINT0_IRQn, NVIC_EncodePriority(5,1,0));


	LPC_GPIO1 -> FIOPIN2 |= 0x78; // turn off LED0 through LED3 (or with 0111 1000)
	while(1)
	{
		sprintf(time,"%2d:%2d:%2d.%d",hours,minutes,seconds,hundred_ms);
		drawString(1,28,time);
	}

	return 0;
}
