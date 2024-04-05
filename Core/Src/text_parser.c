#include "text_parser.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "fatfs.h"

FIL fp;

float ledValues[21];
char tmpBuff[70];

void openTextFile(char *filename) {
	f_open(&fp, filename, FA_READ);
}

void parseTextFile(char* filename) {
	f_gets((TCHAR*)tmpBuff, sizeof(tmpBuff), &fp);
	fillValuesArr(tmpBuff);
}

void fillValuesArr(char line[70]) {
	const char s[2] = ",";
	char* token = strtok(line, s);
  int i = 0;
		
	while(token != NULL) {
		ledValues[i] = (float)atof(token);
		i++;
		token = strtok(NULL, s);
	}
}
