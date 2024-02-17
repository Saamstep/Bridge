#include "main.h"
#include <stdbool.h>
#include <string.h>

unsigned int buffer[LED_COUNT];

/* ****************************************************** */
/* Beams    Parameter                                     */
/* ****************************************************** */
//ledBeam beamBuffer[BRIDGE_BEAMS_NUM] = {
//{20, 0, false}, {10, 0, true}, {8, 0, false},
//{5, 0, true}, {8, 0, false}, {10, 0, true},
//{20, 0, false}, {20, 0, true}, {10, 0, false}
//};

ledBeam beamBuffer[BRIDGE_BEAMS_NUM] = {
{13, 0, false},{13, 0, true},
{13, 0, false},{13, 0, true},
{13, 0, false},{13, 0, true},
{13, 0, false},{13, 0, true},
{13, 0, false}
};

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
}

void clearStrip(void) {
	for(int i = 0; i < LED_COUNT; i++) {
		buffer[i] = LED_COLOR_RESET;
	}
	sendData(buffer);
}

void idle(void) {
	for(int i = 0; i < LED_COUNT; i++) {
		buffer[i] = i % 2 == 0 ? RED : WHITE;
}
	sendData(buffer);
}

/*
60+59 = 119 LEDs
Beams --> 10 LEDs per bema
Total LEDs used --> 110 leds
*/
void seeFrames() {
	int counter = 0;
	int toggle = 0;
	for(int j=0; j<BRIDGE_BEAMS_NUM; j++) {
		toggle = !toggle;
		for(int i=0; i<beamBuffer[j].ledCount; i++) {
			buffer[counter] = toggle ? RED : WHITE;
			counter++;
		}
	}
	for(int k=counter; k<LED_COUNT; k++) {
		buffer[k] = BLUE;
	}
	sendData(buffer);
}

//percentage: 0 to 100 ints
//tells number of leds to turn on
int getFrameValue(ledBeam beam, float percentage) {
	float x = (percentage*beam.ledCount)/100;
	return (int)x;
}

int getColor(ledBeam beam) {
	int percentage = ((float)beam.ledEnabled/(float)beam.ledCount)*100;
	if(percentage >= 0 && percentage <= 33) {
		return BLUE;
	} else if(percentage > 33 && percentage <= 66) {
		return PURPLE;
	} else if(percentage > 66) {
		return RED;
	} else {
		return WHITE;
	}
}

//sets frame to beamBuffer
void setFrames(float data[BRIDGE_BEAMS_NUM]) {
	for(int i=0; i<BRIDGE_BEAMS_NUM; i++) {
		beamBuffer[i].ledEnabled = getFrameValue(beamBuffer[i], data[i]);
	}
}

void writeFrames() {
	clearStrip();
	int counter = 0;
	for(int i=0; i<BRIDGE_BEAMS_NUM; i++) {
		if(!beamBuffer[i].reverse) {
			//===IF LED strip is NOT reversed===
			for(int k=0; k<beamBuffer[i].ledEnabled; k++) {
				buffer[counter] = getColor(beamBuffer[i]);
				counter++;
			}
		counter += beamBuffer[i].ledCount-beamBuffer[i].ledEnabled;
		} else {
			//===IF LED strip IS reversed===
			int min = beamBuffer[i].ledEnabled;
			int COLOR = getColor(beamBuffer[i]);
			for(int k=0; k<beamBuffer[i].ledCount; k++) {
				buffer[counter] = k >= min ? COLOR : LED_COLOR_RESET;
				counter++;
			}
		}
	}
	
	sendData(buffer);
}
