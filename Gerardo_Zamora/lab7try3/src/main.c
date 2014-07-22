//UDP W5200 example
//Base written Nathan Zimmerman

#include "lpc17xx.h" // processor includes
#include "stdint.h" // includes for uint8_t nstuff
#include "W5200/udp.h" //ethernet driver includes
#include "LCD/lcd.h" // includes for ldc
#include "DEV/ece471.h"
#include "stdio.h"
#include "string.h"

static char buffer[50];
static UDP_CONFIG udp_setup;
static signed int sequenceCounter = 1;
int watchDogCounter = 0;
int flag = 1;
int counter1 = 1;

uint8 MAC[6] = {0x00, 0x08, 0xDC, 0x01, 0x02, 0x03};//MAC Address
uint8 IP[4] = {192, 168, 1, 2};//IP Address
uint8 GateWay[4] = {192, 168, 1, 1};//Gateway Address
uint8 SubNet[4] = {255, 255, 255, 0};//SubnetMask Address
uint8_t Dest_IP[4] = {192, 168, 1, 103}; //DST_IP Address

void configDrone(void);
void sendWatchDog(void);
void takeOff(void);
void landDrone(void);

int main(void)
{
	udp_setup.Gateway = GateWay;//Gateway Address
	udp_setup.MAC = MAC;
	udp_setup.Source_IP = IP;//IP Address
	udp_setup.Source_Port= 5556;
	udp_setup.Subnet= SubNet;//SubnetMask Address
	udp_setup.s= 0;

	LCD_SETUP(buffer);

	LPC_GPIO1 -> FIODIR |= 0xFF << 19;
	LPC_GPIO1 -> FIOPIN |= 0xFF << 19;

	udp_socket_init(&udp_setup);

	while(1)
	{
		if(SW0_PRESSED) // Wait for SW0 to be pressed
		{
			if(flag)
			{
				configDrone();
				flag = 0;
			}
		}
		if(!flag)
		{
			sendWatchDog();
			watchDogCounter++;
		}

		if( (watchDogCounter >= 30) && (counter1 <= 2) )
		{
			takeOff();
			watchDogCounter = 0;
			counter1++;
		}



		if(SW1_PRESSED) landDrone(); // if SW1 is pressed, land the drone

	}
}



void configDrone(void)
{
	sprintf(buffer,"AT*CONFIG=%d,\"general:navdata_demo\",\"FALSE\"\r",sequenceCounter);
	udp_send(0,(uint8_t *)buffer,strlen(buffer),Dest_IP, 5556);
	delay_ms(100);
	sequenceCounter++;

	sprintf(buffer,"AT*PMODE=%d,2\r",sequenceCounter);
	udp_send(0,(uint8_t *)buffer,strlen(buffer),Dest_IP, 5556);
	delay_ms(100);
	sequenceCounter++;

	sprintf(buffer,"AT*MISC=%d,2,20,2000,3000\r",sequenceCounter);
	udp_send(0,(uint8_t *)buffer,strlen(buffer),Dest_IP, 5556);
	delay_ms(100);
	sequenceCounter++;

	sprintf(buffer,"AT*CONFIG=%d,\"control:outdoor\",\"FALSE\"\r",sequenceCounter);
	udp_send(0,(uint8_t *)buffer,strlen(buffer),Dest_IP, 5556);
	delay_ms(100);
	sequenceCounter++;

	sprintf(buffer,"AT*CONFIG=%d,\"control:flight_without_shell\",\"FALSE\"\r",sequenceCounter);
	udp_send(0,(uint8_t *)buffer,strlen(buffer),Dest_IP, 5556);
	delay_ms(100);
	sequenceCounter++;

	sprintf(buffer,"AT*CONFIG=%d,\"control:altitude_ max\",\"5000\"\r",sequenceCounter);
	udp_send(0,(uint8_t *)buffer,strlen(buffer),Dest_IP, 5556);
	delay_ms(100);
	sequenceCounter++;

	sprintf(buffer,"AT*CONFIG=%d,\"control:altitude_ min\",\"500\"\r",sequenceCounter);
	udp_send(0,(uint8_t *)buffer,strlen(buffer),Dest_IP, 5556);
	delay_ms(100);
	sequenceCounter++;

	sprintf(buffer,"AT*CONFIG=%d,\"video:bitrate_ctrl_mode\",\"0\"\r",sequenceCounter);
	udp_send(0,(uint8_t *)buffer,strlen(buffer),Dest_IP, 5556);
	delay_ms(100);
	sequenceCounter++;

	LPC_GPIO1 -> FIOPIN &= (~P0_19);
}

void sendWatchDog(void)
{
	sprintf(buffer,"AT*COMWDG=%d\r",sequenceCounter);
	udp_send(0,(uint8_t *)buffer,strlen(buffer),Dest_IP, 5556);
	delay_ms(100);
	sequenceCounter++;
	LPC_GPIO1 -> FIOPIN &= (~P0_20);
}

void takeOff(void)
{
	sprintf(buffer,"AT*REF=%d,290718208\r",sequenceCounter);
	udp_send(0,(uint8_t *)buffer,strlen(buffer),Dest_IP, 5556);
	delay_ms(100);
	sequenceCounter++;
	LPC_GPIO1 -> FIOPIN &= (~P0_21);
}

void landDrone(void)
{
	sprintf(buffer,"AT*REF=%d,290717696\r",sequenceCounter);
	udp_send(0,(uint8_t *)buffer,strlen(buffer),Dest_IP, 5556);
	delay_ms(100);
	sequenceCounter++;
}

