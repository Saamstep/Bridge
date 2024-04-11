#include "main.h"
#include <stdbool.h>
#include <string.h>
#include <math.h>

unsigned int buffer[LED_COUNT];

/* ****************************************************** */
/* Beams    Parameter                                     */
/* ****************************************************** */
ledBeam beamBuffer[BRIDGE_BEAMS_NUM] = {
{11, 0, false},{10, 0, true},
{9, 0, false},{8, 0, true},
{7, 0, false},{7, 0, true},
{6, 0, false}, {6, 0, true},
{6, 0, false},{6, 0, true},
{6, 0, false}, {6, 0, true},
{6, 0, false}, {6, 0, true},
{6, 0, false}, {7, 0, true},
{7, 0, false}, {8, 0, true},
{9, 0, false}, {10, 0, true},
{11, 0, false}
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

int getFrameValue(ledBeam beam, float db) {
	int subdivisions = round(beam.ledCount/4);
	if(db >= 70.0) {
		return beam.ledCount;
	} else if(db < 0) {
		return 0;
	} else if(db >= 0 && db <= 20.0) {
		return subdivisions;
	} else if(db > 20.0 && db <= 45.0) {
		return subdivisions*2;
	} else if(db > 45.0 && db < 70.0) {
		return subdivisions*3;
	}
	return 0;
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

int getColorMulti(ledBeam beam, int ledNum){ 
	int subdivisions = round((((double)beam.ledCount/4.0)));
		if(ledNum == beam.ledCount) {
			return RED;
		}
		if(ledNum > 0 && ledNum <= subdivisions) {
			return GREEN;
		} else if(ledNum > subdivisions && ledNum <= subdivisions*2) {
			return YELLOW;
		} else if(ledNum > subdivisions*2 && ledNum <= subdivisions*3) {
			return ORANGE;
		} else if(ledNum >= subdivisions*3) {
			return RED;
		}
	return LED_COLOR_RESET;
}

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
			for(int k=1; k<=beamBuffer[i].ledEnabled; k++) {
				//buffer[counter] = getColor(beamBuffer[i]);
				buffer[counter] = getColorMulti(beamBuffer[i], k);
				counter++;
		}
		counter += beamBuffer[i].ledCount-beamBuffer[i].ledEnabled;
		} else {
			//===IF LED strip IS reversed===
			//int COLOR = getColor(beamBuffer[i]);
			for(int k=0; k<beamBuffer[i].ledCount-beamBuffer[i].ledEnabled; k++) {
				buffer[counter] = LED_COLOR_RESET;
				counter++;
			}
			int ledIndex = beamBuffer[i].ledCount-(beamBuffer[i].ledCount-beamBuffer[i].ledEnabled);
			for(int k=1; k<=beamBuffer[i].ledEnabled; k++) {
				buffer[counter] = getColorMulti(beamBuffer[i], ledIndex);
				ledIndex--;
				counter++;
			}
		}
	}
	
	sendData(buffer);
}
