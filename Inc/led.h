#ifndef LED_H
#define LED_H

#include <stdbool.h>

/* ****************************************************** */
/* LED Strip Parameters                                   */
/* ****************************************************** */
#define BRIDGE_BEAMS_NUM 9
#define LED_COUNT 120

extern unsigned int buffer[LED_COUNT];

/* ****************************************************** */
/* Bridge    Parameters                                   */
/* ****************************************************** */
typedef struct ledBeam {
	int ledCount;
	int ledEnabled;
	bool reverse;
} ledBeam;

extern ledBeam beamBuffer[BRIDGE_BEAMS_NUM];

/* ****************************************************** */
/* LED Strip Functions		                                */
/* ****************************************************** */
void spi32(unsigned int c);
void sendData(unsigned int *ledArray);
void clearStrip(void);
void idle(void);
void seeFrames(void);
int getFrameValue(ledBeam beam, float percentage);
int getColor(ledBeam beam);
void setFrames(float data[BRIDGE_BEAMS_NUM]);
void writeFrames(void);

#endif
