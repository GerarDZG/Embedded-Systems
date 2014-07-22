/*
===============================================================================
 Name        : main.c
 Author      : Gerardo Zamora
 Version     : 1.0
 Copyright   : Aww Yiss
 Description : Software UART Receive
===============================================================================
 */

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include "lcd.h"


// #define's
#define UART_RECEIVE ((~(LPC_GPIO2 -> FIOPIN) & (1<<1)) == (1<<1)) // P0.2
#define TIMER0_BAUD_RATE_9600 (1302) // Number for Match Reg to generate 9600 baud rate
#define BACKSPACE (0x7F) // DEL in actuality. BS is 0x08. DEL received when BS is hit
#define WIDTH_OF_LETTER (6) // Width of a letter (or character) including a pixel separator
#define HEIGHT_OF_LETTER (9) // Height of a letter (or character) including a pixel separator
#define X_START (1)	// X starting position where characters are written to the LCD
#define Y_START (9) // Y starting position where characters are written to the LCD
#define X_LIMIT (3) // Remaining pixels before reaching JDT1800_WIDTH
#define Y_LIMIT (1) // Remaining pixels before reaching JDT1800_HEIGHT


// Global Variables
uint32_t lcd_x = X_START;
uint32_t lcd_y = Y_START;

uint32_t initialReadFlag = 0;
uint32_t readFlag = 1;
uint32_t sendFlag = 0;
uint32_t sendCharFlag = 0;

uint32_t currentBitPosition = 0;
uint32_t currentBit;
char currentChar = 0;

int main(void)
{
	// Initialize LCD
	lcd_init();
	fillScreen(ST7735_16_WHITE);
	setBackgroundColor16(ST7735_16_WHITE);
	drawString(1,1,"Software UART by GdoZ!");

	// Initialize and Enable TIMER0
	LPC_TIM0 -> MCR |= 0x03;				  // Enable MR0 interrupt and TC reset on match
	LPC_TIM0 -> MR0 |= TIMER0_BAUD_RATE_9600; // To assure correct readings for 9600 baud rate
	LPC_TIM0 -> TCR &= 0xFFFFFFFC; 			  // Timer Clock disabled
	NVIC_EnableIRQ(TIMER0_IRQn);

	// Enable Falling Edge Interrupt on P2.1
	LPC_GPIOINT -> IO2IntEnF |= (1<<1);
	NVIC_EnableIRQ(EINT3_IRQn);

	while(1)
	{
		if(sendCharFlag)
		{
			if (currentChar != BACKSPACE)
			{
				drawChar(lcd_x,lcd_y, currentChar);
				lcd_x += WIDTH_OF_LETTER;

				if(lcd_x > (JDT1800_WIDTH - WIDTH_OF_LETTER))
				{
					lcd_x = X_START;
					lcd_y += HEIGHT_OF_LETTER;
				}

				if(lcd_y > (JDT1800_HEIGHT - HEIGHT_OF_LETTER))
				{
					lcd_x = X_START;
					lcd_y = Y_START;
					fillScreen(ST7735_16_WHITE);
					drawString(1,1,"Software UART by GdoZ!");
				}

				drawChar(lcd_x,lcd_y, '_');
			}
			else
			{
				drawChar(lcd_x,lcd_y, ' ');
				if((lcd_x == X_START) && (lcd_y == Y_START))
				{
					lcd_x = X_START;
					lcd_y = Y_START;
				}
				else if((lcd_x < WIDTH_OF_LETTER) && (lcd_y >= (HEIGHT_OF_LETTER + Y_START)))
				{
					lcd_x = JDT1800_WIDTH - WIDTH_OF_LETTER - X_LIMIT;
					lcd_y -= HEIGHT_OF_LETTER;
				}
				else
				{
					lcd_x -= WIDTH_OF_LETTER;
				}
				drawChar(lcd_x,lcd_y, '_');
			}

			LPC_GPIOINT -> IO2IntEnF |= (1<<1);	// Enable falling edge interrupt on P2.1
			sendCharFlag = 0;
			currentChar = 0;
		}
	}

	return 0;
}

void EINT3_IRQHandler(void)
{
	if(UART_RECEIVE)
	{
		LPC_TIM0 -> TCR &= 0xFFFFFFC; 			// Mask last two bits for enabling TIMER0
		LPC_TIM0 -> TCR |= 1; 					// Enabler TIMER0
		LPC_GPIOINT -> IO2IntEnF &= ~(1<<1);	// Disable falling edge interrupt on P2.1
		LPC_GPIOINT -> IO2IntClr |= (1<<1);		// Clear interrupt flag
	}
}
void TIMER0_IRQHandler(void)
{
	if(sendFlag == 8)
	{
		initialReadFlag = 0;
		currentBitPosition = 0;
		sendFlag = 0;
		sendCharFlag = 1;
		LPC_TIM0 -> TCR &= 0xFFFFFFFE; // Reset Timer Counter
	}
	if((initialReadFlag == 2) && readFlag)
	{
		currentBit = (((LPC_GPIO2 -> FIOPIN) & (1<<1)) == (1<<1))? 1:0;

		currentChar += (currentBit << currentBitPosition);
		readFlag = 0;
		sendFlag++;
		currentBitPosition++;
	}
	else if(initialReadFlag < 2)
	{
		initialReadFlag++;
	}
	else
	{
		readFlag = 1;
	}

	LPC_TIM0 -> IR |= 1;
}

