#ifndef ADC_H
#define ADC_H

#include "stdint.h"

/**
 * @file    adc.h
 * @brief   Header file containing function declarations for configuring and using
 *          the Analog-to-Digital Converter (ADC) on the MKL25Z4 microcontroller.
 * @author  Suhas Srinivasa Reddy
 * @date    1st Dec 2023
 */

/**
 * @brief   Initializes the Analog-to-Digital Converter (ADC).
 * @return  None
 *
 * This function sets up the necessary configurations for the ADC, such as
 * selecting the appropriate channels and configuring resolution.
 */
void Init_ADC(void);

/**
 * @brief   Reads a waveform from the Analog-to-Digital Converter (ADC) and performs analysis.
 * @details Initiates an ADC conversion, reads the waveform samples, and performs various analyses,
 *          including autocorrelation detection, sorting, and computation of statistical measures.
 *          When a complete waveform is processed (1024 samples), it prints the minimum value,
 *          maximum value, average value, and estimated frequency to the console.
 */
void read_waveform(void);

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
void bubbleSort(uint16_t arr[], uint16_t n);

/**
 * @brief   Swaps the values of two pointers.
 * @details Given two pointers to uint16_t values, this function swaps the values
 *          they point to. It is commonly used for sorting algorithms and similar
 *          operations where the order of elements needs to be changed.
 *
 * @param   a: Pointer to the first uint16_t value.
 * @param   b: Pointer to the second uint16_t value.
 */
void swap(uint16_t *a, uint16_t *b);

/**
 * @brief   Computes the average value of an array of uint16_t values.
 * @details This function calculates the average value of an array by summing up
 *          all the elements and dividing the sum by the total number of elements.
 *          The result is stored in the global variable 'avg'.
 */
void compute_avg(void);

#endif  // ADC_H
