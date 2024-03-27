#include "musical_bridge.h"
#include "led.h"

float32_t divideAndMax (float32_t *data, int start, int end) {
		uint32_t indexValue;
		float32_t maxValue;
		int numVals = end - start + 1;
		float32_t vals[numVals];
		for (int i = start; i <= end; i++ ){
				vals[i-start] = data[i];
		}
		arm_max_f32(vals, numVals, &maxValue, &indexValue);
		return maxValue;
}

//Function to downsample filtered audio based on preset downsampling factor
uint32_t downsampleAudio(float32_t *signal, uint32_t numSamples){

    uint32_t dsNumSamples = numSamples / DOWNFACTOR;
    //printf("Down Sample Number: %d\n", dsNumSamples);
    //printf("Size of signal : %d\n",(int) (sizeof(&signal) / sizeof(&signal[0])));
    float32_t *tempSignal = (float32_t *)malloc(dsNumSamples * sizeof(float32_t));
    for (uint32_t i = 0; i < dsNumSamples; i++) {
        tempSignal[i] = signal[i * DOWNFACTOR];
        //printf("iteration %d complete\n", i);
    }
    for (uint32_t i = 0; i < dsNumSamples; i++) {
        signal[i] = tempSignal[i];
    }
    free(tempSignal);
		return dsNumSamples;
}

//Function to segment downsampled audio into smaller 
void analyzeAudio(float32_t *signal, float32_t *intensity) {
    //printf("Entering audio analysis\n");
    arm_cfft_radix4_instance_f32 S;   /* ARM CFFT module */
    float32_t maxValue[21];                /* Max FFT value is stored here */
		float32_t sum;
		float32_t *output = (float32_t *)malloc(DFT_SIZE * sizeof(float32_t));
		arm_cfft_radix4_init_f32(&S, DFT_SIZE, 0, 1);
		arm_cfft_radix4_f32(&S, signal);
		arm_cmplx_mag_f32(signal, output, DFT_SIZE);
		maxValue[0] = divideAndMax(output, 17, 20);
		maxValue[1] = divideAndMax(output, 20, 24);
		maxValue[2] = divideAndMax(output, 24, 28);
		maxValue[3] = divideAndMax(output, 28, 33);
		maxValue[4] = divideAndMax(output, 33, 39);
		maxValue[5] = divideAndMax(output, 39, 47);
		maxValue[6] = divideAndMax(output, 47, 55);
		maxValue[7] = divideAndMax(output, 55, 65);
		maxValue[8] = divideAndMax(output, 65, 78);
		maxValue[9] = divideAndMax(output, 78, 92);
		maxValue[10] = divideAndMax(output, 92, 109);
		maxValue[11] = divideAndMax(output, 109, 130);
		maxValue[12] = divideAndMax(output, 130, 154);
		maxValue[13] = divideAndMax(output, 154, 183);
		maxValue[14] = divideAndMax(output, 183, 218);
		maxValue[15] = divideAndMax(output, 218, 259);
		maxValue[16] = divideAndMax(output, 259, 307);
		maxValue[17] = divideAndMax(output, 307, 365);
		maxValue[18] = divideAndMax(output, 365, 434);
		maxValue[19] = divideAndMax(output, 434, 516);
		maxValue[20] = divideAndMax(output, 516, 614);
		
		for (int i = 0; i < (sizeof(output)/sizeof(output[0])); i++) {
				sum = sum + output[i];
		}
		
		for (int i = 0; i < 21; i++) {
				intensity[i] = (maxValue[i] / sum) * 100;
		}
		
}
