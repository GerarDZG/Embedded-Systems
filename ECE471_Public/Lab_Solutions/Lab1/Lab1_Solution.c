//Nathan Zimmerman
//Lab 1 Solution

#include "LPC17xx.h" //Include necessary LPC1769 definitions such as LPC_GPIO..ect

const unsigned long sequence[] = {1<<19,1<<20, 1<<21,1<<22, 1<<23,1<<24,1<<25,1<<26,1<<25,1<<24,1<<23,1<<22,1<<21,1<<20}; //Define sequence

int main(void) {
	volatile int i=0,u=0,SequenceOn=1; //Define count bits and bool
	LPC_GPIO1 -> FIODIR |= 0xFF<<19; //Define LEDs as outputs
	while (1) { //While 1 is equal to 1 (aka forever)
		if(SequenceOn)
			LPC_GPIO1 -> FIOPIN = ~(sequence[u]); //Turn LED on in sequence
		u = (u+1)%13;// Count 14 times then reset
		for (i = 0; i < 500000; i++); // Sequence Delay
		if(!(((LPC_GPIO0->FIOPIN)&(0xF<<2))==(0xF<<2))) { // If any button is pressed
			SequenceOn ^= 1; //Toggle sequence on
			LPC_GPIO1 -> FIOPIN |=(0xFF<<19); // Turn LEDs Off
			for (i = 0; i < 1500000; i++); // Button de-bounce delay
		}
	}
return 0;
}
