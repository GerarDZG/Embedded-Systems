//UDP W5200 example
//Base written Nathan Zimmerman

#include "lpc17xx.h" // processor includes
#include "stdint.h" // includes for uint8_t nstuff
#include "W5200/udp.h" //ethernet driver includes
#include "LCD/lcd.h" // includes for ldc
#include "DEV/ece471.h"
#include "stdio.h"
#include "string.h"

static char buffer[40];
static UDP_CONFIG udp_setup;

uint8 MAC[6] = {0x00, 0x08, 0xDC, 0x01, 0x02, 0x03};//MAC Address
uint8 IP[4] = {192, 168, 1, 23};//IP Address
uint8 GateWay[4] = {192, 168, 1, 1};//Gateway Address
uint8 SubNet[4] = {255, 255, 255, 0};//SubnetMask Address
uint8_t Dest_IP[4] = {192, 168, 1, 103}; //DST_IP Address

int main(void)
{
	udp_setup.Gateway = GateWay;//Gateway Address
	udp_setup.MAC = MAC;
	udp_setup.Source_IP = IP;//IP Address
	udp_setup.Source_Port= 5556;
	udp_setup.Subnet= SubNet;//SubnetMask Address
	udp_setup.s= 0;

	LCD_SETUP(buffer);

	udp_socket_init(&udp_setup);

	while(1)
	{
		while(!SW0_PRESSED); // Wait for SW0 to be pressed
		sprintf(buffer,"uBer UdP pAcKeT oF sUpEr AmAzInG aWeSoMeNeSs");
		udp_send(0,(uint8_t *)buffer,strlen(buffer),Dest_IP, 5556);
		delay_ms(100);
	}
}







