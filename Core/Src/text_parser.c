#include "text_parser.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "fatfs.h"

FIL fp;

float ledValues[21] = {100.0, 100.0, 100.0, 100.0, 100.0, 100.0, 100.0, 100.0, 100.0, 100.0, 100.0, 100.0, 100.0, 100.0, 100.0, 100.0, 100.0, 100.0, 100.0, 100.0, 100.0};
char tmpBuff[2][16000];
int buffNum = 0;
bool buffer_full[2] = {false};

extern bool valuesPlayed;

void openTextFile(char *filename) {
	f_open(&fp, filename, FA_READ);
}

void parseTextFile(char* filename) {
		f_gets((TCHAR*)tmpBuff[buffNum], sizeof(tmpBuff[buffNum]), &fp);
		buffer_full[buffNum] = true;
}

void fillValuesArr(char* line) {
	char* token = strtok(line, ",");
	int i = 0;
	
	while(token != NULL && i < 21) {
		ledValues[i] = (float)atof(token);
		token = strtok(NULL, ",");
		i++;
	}
}
