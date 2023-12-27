#ifndef WAVEFORMS_H
#define WAVEFORMS_H

#include "stdint.h"

/**
 * @file    waveforms.h
 * @brief   Header file containing function declarations and constants for waveform generation.
 * @author  [Author Name]
 * @date    [Date]
 */
#define NUM_STEPS 1024

extern uint16_t TriangleTable[NUM_STEPS];
extern uint16_t SquareTable[NUM_STEPS];
extern uint16_t SineTable[NUM_STEPS];

/**
 * @brief   Initializes a Triangle waveform table with a specific frequency and sample size.
 * @details This function generates a Triangle waveform with a frequency of 800Hz and a sample
 *          size of TRIANGLE_SAMPLE_SIZE. The waveform is stored in the global array
 *          TriangleTable for further use in the system. Each waveform cycle is divided into
 *          two halves, with ascending and descending values based on the step size.
 */
void Init_TriangleTable(void);

/**
 * @brief   Initializes a Square waveform table with a specific frequency and sample size.
 * @details This function generates a Square waveform with a frequency of 400Hz and a sample
 *          size of SQUARE_SAMPLE_SIZE. The waveform is stored in the global array
 *          SquareTable for further use in the system. Each waveform cycle is divided into
 *          two halves, with one half having the minimum DAC input value and the other half
 *          having the maximum DAC input value.
 */
void Init_SquareTable(void);

/**
 * @brief   Initializes a Sine waveform table with a specific frequency and sample size.
 * @details This function generates a Sine waveform with a frequency of 600Hz and a sample
 *          size of SINE_SAMPLE_SIZE. The waveform is stored in the global array
 *          SineTable for further use in the system. Each waveform cycle is divided into
 *          two halves, with positive and negative values based on the sine function and
 *          a scaling factor.
 */
void Init_SineTable(void);

#endif  // WAVEFORMS_H
