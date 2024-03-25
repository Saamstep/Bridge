/*
 * wav_player.c
 *
 *  Created on: 17 Apr 2020
 *      Author: Mohamed Yaqoob
 */

#include "wav_player.h"
#include "audioI2S.h"
#include "fatfs.h"
#include "bridge2.h"
#include "ff.h"


//WAV File System variables
static FIL wavFile;
char** wavListBuff = NULL;
int num_of_songs = 0;

//WAV Audio Buffer
static uint32_t fileLength;
#define AUDIO_BUFFER_SIZE  4096*1
#define MAX_UINT8_VAL 255.0
static uint8_t audioBuffer[AUDIO_BUFFER_SIZE];
double *doubleDataBuff;
double *freq, *intensity;

static __IO uint32_t audioRemainSize = 0;

//WAV Player
static uint32_t samplingFreq;
static UINT playerReadBytes = 0;
static bool isFinished=0;
//WAV Player process states
typedef enum
{
  PLAYER_CONTROL_Idle=0,
  PLAYER_CONTROL_HalfBuffer,
  PLAYER_CONTROL_FullBuffer,
  PLAYER_CONTROL_EndOfFile,
}PLAYER_CONTROL_e;
static volatile PLAYER_CONTROL_e playerControlSM = PLAYER_CONTROL_Idle;

static void wavPlayer_reset(void)
{
  audioRemainSize = 0;
  playerReadBytes = 0;
}

/* 
* @brief Read and fill buff of all WAV files to play.
* @retval returns void
*/
void getAllWav() {
{
    FRESULT fr;     /* Return value */
    DIR dj;         /* Directory object */
    FILINFO fno;    /* File information */
		int i = 0;

    fr = f_findfirst(&dj, &fno, "", "*.wav"); /* Start to search for files */

    while (fr == FR_OK && fno.fname[0]) {         /* Repeat while an item is found */
				num_of_songs++;
        fr = f_findnext(&dj, &fno);               /* Search for next item */
    }
		
		wavListBuff = (char**)malloc(num_of_songs*sizeof(char*));
		f_closedir(&dj);
		
		fr = f_findfirst(&dj, &fno, "", "*.wav"); /* Start to search for files */

    while (fr == FR_OK && fno.fname[0]) {         /* Repeat while an item is found */
			wavListBuff[i] = (char*)malloc(strlen(fno.fname+1)*sizeof(char));
			strcpy(wavListBuff[i], fno.fname);
			i++;
      fr = f_findnext(&dj, &fno);               /* Search for next item */
    }

    f_closedir(&dj);
}
}

/**
 * @brief Select WAV file to play
 * @retval returns true when file is found in USB Drive
 */
bool wavPlayer_fileSelect(const char* filePath)
{
  WAV_HeaderTypeDef wavHeader;
  UINT readBytes = 0;
  //Open WAV file
  if(f_open(&wavFile, filePath, FA_READ) != FR_OK)
  {
    return false;
  }
  //Read WAV file Header
  f_read(&wavFile, &wavHeader, sizeof(wavHeader), &readBytes);
  //Get audio data size
  fileLength = wavHeader.FileSize;
  //Play the WAV file with frequency specified in header
  samplingFreq = wavHeader.SampleRate;
  return true;
}

/**
 * @brief WAV File Play
 */
void wavPlayer_play(void)
{
  isFinished = false;
  //Initialise I2S Audio Sampling settings
  audioI2S_init(samplingFreq);
  //Read Audio data from USB Disk
  f_lseek(&wavFile, 0);
  f_read (&wavFile, &audioBuffer[0], AUDIO_BUFFER_SIZE, &playerReadBytes);
	size_t numSamples = playerReadBytes;
	doubleDataBuff = (double *)malloc(numSamples * sizeof(double));
	freq = (double *)malloc(numSamples * sizeof(double));
	intensity = (double *)malloc(numSamples * sizeof(double));
	//if (doubleDataBuff == NULL) exit(1);
	
	for (size_t i = 0; i < numSamples; i++) {
		doubleDataBuff[i] = (double)audioBuffer[i] / MAX_UINT8_VAL;
	}
	//audioProcess(doubleDataBuff, freq, intensity);
  audioRemainSize = fileLength - playerReadBytes;
  //Start playing the WAV
  audioI2S_play((uint16_t *)&audioBuffer[0], AUDIO_BUFFER_SIZE);
}

/**
 * @brief Process WAV
 */
void wavPlayer_process(void)
{
  switch(playerControlSM)
  {
  case PLAYER_CONTROL_Idle:
    break;

  case PLAYER_CONTROL_HalfBuffer:
    playerReadBytes = 0;
    playerControlSM = PLAYER_CONTROL_Idle;
    f_read (&wavFile, &audioBuffer[0], AUDIO_BUFFER_SIZE/2, &playerReadBytes);
    if(audioRemainSize > (AUDIO_BUFFER_SIZE / 2))
    {
      audioRemainSize -= playerReadBytes;
    }
    else
    {
      audioRemainSize = 0;
      playerControlSM = PLAYER_CONTROL_EndOfFile;
    }
    break;

  case PLAYER_CONTROL_FullBuffer:
    playerReadBytes = 0;
    playerControlSM = PLAYER_CONTROL_Idle;
    f_read (&wavFile, &audioBuffer[AUDIO_BUFFER_SIZE/2], AUDIO_BUFFER_SIZE/2, &playerReadBytes);
    if(audioRemainSize > (AUDIO_BUFFER_SIZE / 2))
    {
      audioRemainSize -= playerReadBytes;
    }
    else
    {
      audioRemainSize = 0;
      playerControlSM = PLAYER_CONTROL_EndOfFile;
    }
    break;

  case PLAYER_CONTROL_EndOfFile:
    f_close(&wavFile);
    wavPlayer_reset();
    isFinished = true;
    playerControlSM = PLAYER_CONTROL_Idle;
    break;
  }
}

/**
 * @brief WAV stop
 */
void wavPlayer_stop(void)
{
  audioI2S_stop();
  isFinished = true;
	free(doubleDataBuff);
	free(freq);
	free(intensity);
}

/**
 * @brief WAV pause/resume
 */
void wavPlayer_pause(void)
{
  audioI2S_pause();
}
void wavPlayer_resume(void)
{
  audioI2S_resume();
}

/**
 * @brief isEndofFile reached
 */
bool wavPlayer_isFinished(void)
{
  return isFinished;
}

/**
 * @brief Half/Full transfer Audio callback for buffer management
 */
void audioI2S_halfTransfer_Callback(void)
{
  playerControlSM = PLAYER_CONTROL_HalfBuffer;
}
void audioI2S_fullTransfer_Callback(void)
{
  playerControlSM = PLAYER_CONTROL_FullBuffer;
//  audioI2S_changeBuffer((uint16_t*)&audioBuffer[0], AUDIO_BUFFER_SIZE / 2);
}

