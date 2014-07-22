//Nathan Zimmerman
//Example Blank Project
//Blink LED

#include "LPC17xx.h" //Include necessary LPC1769 definitions such as LPC_GPIO..ect

#define LED0	(1<<19) // Define the 19th bit as LED0

int main(void) {

	volatile int i = 0; //Define arbitrary bit for counting

	LPC_GPIO1 -> FIODIR |= LED0; //Define LED0 as output

	while (1) { //While 1 is equal to 1 (aka forever)

		LPC_GPIO1 -> FIOPIN ^= LED0; //Toggle LED0

		for (i = 0; i < 2000000; i++); //Count up to 2000000

	}

	return 0;

}

