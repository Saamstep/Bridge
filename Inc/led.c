#include "main.h"
#include "config.h"
#include "led.h"

unsigned int buffer[LED_COUNT];

void spi32(unsigned int c) {
	// Create pin state variable
	GPIO_PinState pin;
	
	// Send out 32 bits of c
	for(int i = 0; i < 32; i++) {
		// Use a mask to compare c to 0 and 1
		if((c & 0x80000000) == 0) {
			pin = GPIO_PIN_RESET;
		} else {
			pin = GPIO_PIN_SET;
		}
		c = c << 1;
		
		// Write data to GPIO pins
		HAL_GPIO_WritePin(SDO_GPIO_Port, SDO_Pin, pin);
		HAL_GPIO_WritePin(SCK_GPIO_Port, SCK_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(SCK_GPIO_Port, SCK_Pin, GPIO_PIN_RESET);
	}
}

void sendData(unsigned int *ledArray) {
	spi32(0);
	for(int i = 0; i < LED_COUNT; i++) {
		spi32(ledArray[i]);
	}
	spi32(0xffffffff);
	//spi32(0xffffffff);
}

void clearStrip(void) {
	for(int i = 0; i < LED_COUNT; i++) {
		buffer[i] = LED_COLOR_RESET;
	}
	sendData(buffer);
}
