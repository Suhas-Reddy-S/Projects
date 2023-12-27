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
 * @file    dma.c
 * @brief   This file contains functions for configuring and using DMA (Direct Memory Access)
 *          to play back audio waveforms using a DAC (Digital-to-Analog Converter).
 *          It supports square, sine, and triangle waves and utilizes a callback
 *          function to switch between different waveforms.
 *
 * @author  Suhas Reddy S
 * @date    1st Dec 2023
 *
 */

#include <stdint.h>
#include <MKL25Z4.h>
#include <waveforms.h>
#include "timer.h"
#include "dac.h"
#include "dma.h"
#include "systick.h"

uint16_t *Reload_DMA_Source = 0;        // Source pointer for dma
uint32_t Reload_DMA_Byte_Count = 0;
uint32_t DMA_Playback_Count = 0;

// Enumeration for different waveform types
typedef enum waves {
	SQUARE, SINE, TRIANGLE
} wave_t;

// Set initial wave as Square
wave_t current_wave = SQUARE;

/**
 * @brief   Initializes DMA (Direct Memory Access) for audio playback.
 * @param   source: Pointer to the source data array (waveform samples).
 * @param   count: Number of samples in the waveform.
 * @return  None
 *
 * This function sets up the DMA configuration for audio playback using a DAC (Digital-to-Analog Converter).
 * It saves the source data and byte count, configures DMA channel properties, enables DMA interrupts,
 * and associates DMA with a peripheral trigger (TPM0 overflow in this case). The function assumes a 16-bit
 * data size and increments the source address after each transfer.
 *
 * Written by C.Dean and modified by Suhas Srinivasa Reddy.
 *
 */
void Init_DMA_For_Playback(uint16_t *source, uint32_t count) {
	// Save reload information
	Reload_DMA_Source = source;
	Reload_DMA_Byte_Count = count * 2;

	// Gate clocks to DMA and DMAMUX
	SIM->SCGC7 |= SIM_SCGC7_DMA_MASK;
	SIM->SCGC6 |= SIM_SCGC6_DMAMUX_MASK;

	// Disable DMA channel to allow configuration
	DMAMUX0->CHCFG[0] = 0;

	// Generate DMA interrupt when done
	// Increment source, transfer words (16 bits)
	// Enable peripheral request
	DMA0->DMA[0].DCR = DMA_DCR_EINT_MASK | DMA_DCR_SINC_MASK
			| DMA_DCR_SSIZE(2) | DMA_DCR_DSIZE(2) |
			DMA_DCR_ERQ_MASK | DMA_DCR_CS_MASK;

	// Configure NVIC for DMA ISR
	NVIC_SetPriority(DMA0_IRQn, 2);
	NVIC_ClearPendingIRQ(DMA0_IRQn);
	NVIC_EnableIRQ(DMA0_IRQn);

	// Enable DMA MUX channel with TPM0 overflow as trigger
	DMAMUX0->CHCFG[0] = DMAMUX_CHCFG_SOURCE(54);
}

/**
 * @brief   Initiates the DMA playback process.
 * @return  None
 *
 * This function initializes the source and destination pointers for DMA, sets the byte count,
 * clears the DMA done flag, and enables the DMA channel. It effectively starts the DMA transfer
 * to play back audio waveforms from the source buffer to the DAC (Digital-to-Analog Converter).
 */
void Start_DMA_Playback(void) {
	// initialize source and destination pointers
	DMA0->DMA[0].SAR = DMA_SAR_SAR((uint32_t ) Reload_DMA_Source);
	DMA0->DMA[0].DAR = DMA_DAR_DAR((uint32_t) (&(DAC0->DAT[0])));
	// byte count
	DMA0->DMA[0].DSR_BCR = DMA_DSR_BCR_BCR(Reload_DMA_Byte_Count);
	// clear done flag
	DMA0->DMA[0].DSR_BCR &= ~DMA_DSR_BCR_DONE_MASK;
	// set enable flag
	DMAMUX0->CHCFG[0] |= DMAMUX_CHCFG_ENBL_MASK;

}

void DMA0_IRQHandler(void) {
	// Clear done flag
	DMA0->DMA[0].DSR_BCR |= DMA_DSR_BCR_DONE_MASK;
	// Start the next DMA playback cycle
	Start_DMA_Playback();

}

/**
 * @brief   Initializes and plays audio tones (waveforms) using DMA and a DAC.
 * @return  None
 *
 * This function sets up the necessary components for audio playback. It initializes the DAC,
 * waveform tables (square, triangle, and sine), DMA for playback with a specific waveform,
 * system tick timer, and a TPM (Timer/PWM Module). Finally, it starts both the TPM and DMA
 * playback, allowing continuous playback of audio tones using Direct Memory Access.
 */
void Play_Tone_with_DMA(void) {
	Init_DAC();
	Init_SquareTable();
	Init_SineTable();
	Init_TriangleTable();
	Init_DMA_For_Playback(SquareTable, 960);
	Init_SysTick();
	Init_TPM();
	Start_TPM();
	Start_DMA_Playback();
}

/**
 * @brief   Callback function to switch between different audio waveforms.
 * @return  None
 *
 * This function is a callback used in the audio playback system to switch between
 * different waveform types (square, sine, and triangle). It updates the global variable
 * 'current_wave' and adjusts the DMA source pointer ('Reload_DMA_Source') accordingly.
 * The function is typically called when a designated event occurs, allowing dynamic
 * waveform switching during audio playback.
 */
void CallBack_Function(void) {

	switch (current_wave) {
	case SQUARE: {
		current_wave = SINE;            // Set next wave
		Reload_DMA_Source = SineTable;  // Update source to the next waveform
		break;
	}
	case SINE: {
		current_wave = TRIANGLE;
		Reload_DMA_Source = TriangleTable;
		break;
	}
	case TRIANGLE: {
		current_wave = SQUARE;
		Reload_DMA_Source = SquareTable;
		break;
	}
	}
}
