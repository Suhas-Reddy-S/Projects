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
 * @file    adc.c
 * @brief   This file contains functions for configuring and utilizing the Analog-to-Digital
 *          Converter (ADC) on the MKL25Z4 microcontroller. It provides functionality for
 *          initializing the ADC, performing analog-to-digital conversions, and handling
 *          related tasks.
 * @author  Suhas Reddy S
 * @date    1st Dec 2023
 */

#include "MKL25Z4.h"
#include "adc.h"
#include "autocorrelate.h"
#include "stdio.h"

#define ADC_POS (20)
#define ADC_CHSEL (23)
#define BUFFER_SIZE (1024)
#define ZERO (0)
#define MODE_SELECTION (3)   // 3 selects 16 ADC output reg
#define TRIGGER (1)          // 1 selects hardware trigger
#define HARDWARE_SEL (9)     // 9 selects TPM1 as harware trigger
#define ALTTRG (1)           // 1 select ADC to use alternative trigger
#define ADC_SAM_FREQ (48000)

int i = 0;    // Integer to track ADC buffer idx
uint16_t ADC_buffer[BUFFER_SIZE], max, min, avg;
uint32_t sum;

void Init_ADC(void) {

	SIM->SCGC6 |= SIM_SCGC6_ADC0_MASK;
//	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;

	// Select analog for pin
//	PORTE->PCR[ADC_POS] &= ~PORT_PCR_MUX_MASK;
//	PORTE->PCR[ADC_POS] |= PORT_PCR_MUX(0);

	// Low power configuration, long sample time, 16 bit single-ended conversion, bus clock input
	ADC0->CFG1 = ADC_CFG1_ADLPC_MASK | ADC_CFG1_ADLSMP_MASK
			| ADC_CFG1_MODE(MODE_SELECTION) | ADC_CFG1_ADICLK(BUFFER_SIZE);
	// Hawrdware trigger, compare function disabled, DMA disabled, voltage references VREFH and VREFL
	ADC0->SC2 |= ADC_SC2_REFSEL(ZERO) | ADC_SC2_ADTRG(TRIGGER);

	// Set TMP1 as the hardware trigger
	SIM->SOPT7 |= SIM_SOPT7_ADC0TRGSEL(HARDWARE_SEL) | SIM_SOPT7_ADC0ALTTRGEN(ALTTRG);
}

/**
 * @brief   Reads a waveform from the Analog-to-Digital Converter (ADC) and performs analysis.
 * @details Initiates an ADC conversion, reads the waveform samples, and performs various analyses,
 *          including autocorrelation detection, sorting, and computation of statistical measures.
 *          When a complete waveform is processed (1024 samples), it prints the minimum value,
 *          maximum value, average value, and estimated frequency to the console.
 */
void read_waveform(void) {
	ADC0->SC1[0] = ADC_SC1_ADCH(ADC_CHSEL);
	while (!(ADC0->SC1[0] & ADC_SC1_COCO_MASK))
		;
	ADC_buffer[i] = (int16_t) ADC0->R[0];
	i++;
	if (i == 1024) {
		i = 0;
		int Samples = autocorrelate_detect_period(ADC_buffer, 1024,
				kAC_16bps_unsigned);
		bubbleSort(ADC_buffer, BUFFER_SIZE);        // Sort the ADC bufer array
		// Set min and max value and average from sorted ADC_buffer
		min = ADC_buffer[ZERO];
		max = ADC_buffer[BUFFER_SIZE-1];
		compute_avg();
		printf("\n\rMin = %u, Max = %u, Average = %u, Frequency = %u, Period: %u samples.\n\r", min, max, avg, ADC_SAM_FREQ/Samples, Samples);
	}

}

/**
 * @brief   Swaps the values of two pointers.
 * @details Given two pointers to uint16_t values, this function swaps the values
 *          they point to. It is commonly used for sorting algorithms and similar
 *          operations where the order of elements needs to be changed.
 *
 * @param   a: Pointer to the first uint16_t value.
 * @param   b: Pointer to the second uint16_t value.
 */
void swap(uint16_t *a, uint16_t *b) {
	int16_t temp = *a;
	*a = *b;
	*b = temp;
}

/**
 * @brief   Sorts an array of uint16_t values using the Bubble Sort algorithm.
 * @details This function arranges the elements in ascending order using the Bubble Sort
 *          algorithm. It repeatedly steps through the list, compares adjacent elements,
 *          and swaps them if they are in the wrong order. The process is repeated until
 *          the entire array is sorted.
 *
 * @param   arr: Pointer to the array of uint16_t values to be sorted.
 * @param   n:   Number of elements in the array.
 */
void bubbleSort(uint16_t arr[], uint16_t n) {
	for (int i = 0; i < n - 1; i++) {
		for (int j = 0; j < n - i - 1; j++) {
			if (arr[j] > arr[j + 1]) {
				swap(&arr[j], &arr[j + 1]);
			}
		}
	}
}

/**
 * @brief   Computes the average value of an array of uint16_t values.
 * @details This function calculates the average value of an array by summing up
 *          all the elements and dividing the sum by the total number of elements.
 *          The result is stored in the global variable 'avg'.
 */
void compute_avg(void) {
	for (int i = 0; i < 1024; i++) {
		sum+= ADC_buffer[i];
	}
	avg = sum/1024;
}
