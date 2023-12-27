/*******************************************************************************
 * Copyright (C) 2023 by Suhas Srinivasa Reddy
 *
 * Redistribution, modification, or use of this software in source or binary
 * forms is permitted as long as the files maintain this copyright. Users are
 * permitted to modify this and use it to learn about the field of embedded
 * software. Suhas Srinivasa Reddy and the University of Colorado are not liable
 * for any misuse of this material.
 ******************************************************************************/

/**
 * @file    waveforms.c
 * @brief   This file contains functions to generate and initialize different waveform tables,
 *          including Triangle, Square, and Sine waves. The generated waveforms are stored in
 *          global arrays (TriangleTable, SquareTable, SineTable) for further use in the system.
 *          Each waveform table is designed with a specific frequency and sample size.
 * @author  Suhas Reddy S
 * @date    1st Dec 2023
 */
#include <waveforms.h>
#include "fp_trig.h"
#include "stdio.h"

#define BUFFER_SAMPLES 960U
#define TRIANGLE_SAMPLE_SIZE 120
#define SINE_SAMPLE_SIZE 160
#define SQUARE_SAMPLE_SIZE 240
#define TRIANGLE_STEP_SIZE 68
#define SINE_STEP_SIZE 80
#define MAX_DAC_IP 4074
#define MIN_DAC_IP 0

uint16_t TriangleTable[NUM_STEPS];
uint16_t SquareTable[NUM_STEPS];
uint16_t SineTable[NUM_STEPS];

/**
 * @brief   Initializes a Triangle waveform table with specific frequency and sample size.
 * @details This function generates a Triangle waveform with a frequency of 800Hz and a sample
 *          size of `TRIANGLE_SAMPLE_SIZE`. The waveform is stored in the global array
 *          `TriangleTable` for further use in the system. Each waveform cycle is divided into
 *          two halves, with ascending and descending values based on the step size.
 */
void Init_TriangleTable(void) {
	uint16_t n;

	for (uint16_t i = 0; i < BUFFER_SAMPLES; i += TRIANGLE_SAMPLE_SIZE) {
			for (n = 0; n < TRIANGLE_SAMPLE_SIZE/2; n++) {
				TriangleTable[n+i] = n*TRIANGLE_STEP_SIZE;
				TriangleTable[n + i + TRIANGLE_SAMPLE_SIZE/2] = (TRIANGLE_SAMPLE_SIZE/2-n)*TRIANGLE_STEP_SIZE;
			}

		}
	printf("\n\rGenerated %d samples at frequency 800Hz for Triangle Wave. Computed Period: %u samples.\n\r", BUFFER_SAMPLES, TRIANGLE_SAMPLE_SIZE);
}

/**
 * @brief   Initializes a Square waveform table with a specific frequency and sample size.
 * @details This function generates a Square waveform with a frequency of 400Hz and a sample
 *          size of `SQUARE_SAMPLE_SIZE`. The waveform is stored in the global array
 *          `SquareTable` for further use in the system. Each waveform cycle is divided into
 *          two halves, with one half having the minimum DAC input value and the other half
 *          having the maximum DAC input value.
 */
void Init_SquareTable(void) {
	uint16_t n;

	for (int i = 0; i < BUFFER_SAMPLES; i += SQUARE_SAMPLE_SIZE) {
		for (n = i; n < i + SQUARE_SAMPLE_SIZE/2; n++) {
			SquareTable[n] = MIN_DAC_IP;
			SquareTable[n + SQUARE_SAMPLE_SIZE/2] = MAX_DAC_IP;
		}

	}
	printf("\n\rGenerated %d samples at frequency 400Hz for Square Wave. Computed Period: %u samples.\n\r", BUFFER_SAMPLES, SQUARE_SAMPLE_SIZE);
}

/**
 * @brief   Initializes a Sine waveform table with a specific frequency and sample size.
 * @details This function generates a Sine waveform with a frequency of 600Hz and a sample
 *          size of `SINE_SAMPLE_SIZE`. The waveform is stored in the global array
 *          `SineTable` for further use in the system. Each waveform cycle is divided into
 *          two halves, with positive and negative values based on the sine function and
 *          a scaling factor.
 */
void Init_SineTable(void) {
	uint16_t n;

	for (uint16_t i = 0; i < BUFFER_SAMPLES; i += SINE_SAMPLE_SIZE) {
			for (n = 0; n < SINE_SAMPLE_SIZE/2; n++) {
				SineTable[n+i] = fp_sin(n*SINE_STEP_SIZE) + TRIG_SCALE_FACTOR;
				SineTable[n+i+SINE_SAMPLE_SIZE/2] = - fp_sin(n*SINE_STEP_SIZE) + TRIG_SCALE_FACTOR;
			}

		}
	printf("\n\rGenerated %d samples at frequency 600Hz for Sine Wave. Computed Period: %u samples.\n\r", BUFFER_SAMPLES, SINE_SAMPLE_SIZE);
}
