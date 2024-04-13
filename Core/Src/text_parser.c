#include "text_parser.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "fatfs.h"
#include <math.h>

FIL fp;
UINT br;

float ledValues[21] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
char tmpBuff[64000];
bool flag;
//int buffNum = 0;
//bool buffer_full[2] = {false};
int counting;

extern bool valuesPlayed;

void openTextFile(char *filename) {
	f_open(&fp, filename, FA_READ);
}

void parseTextFile(char* filename) {
		if(flag) {
				f_gets((TCHAR*)tmpBuff, sizeof(tmpBuff), &fp);
				fillValuesArr(tmpBuff);
				counting++;
				flag = false;
		}
		//f_read(&fp, tmpBuff[buffNum], sizeof(tmpBuff[buffNum]), &br); 
}

void fillValuesArr(char* line) {
	char* token = strtok(line, ",");
	int i = 0;
	
	while(token != NULL && i < 21) {
		if(i > 6 && i < 16) {
			ledValues[i] = 1.15*(20*log((float)atof(token)));
		} else {
			ledValues[i] = 20*log((float)atof(token));
		}
		token = strtok(NULL, ",");
		
		i++;
	}
}
