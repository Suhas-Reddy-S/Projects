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
 * @file    timer.c
 * @brief   This file contains functions for configuring and using Timers/PWM Modules (TPM)
 *          on the MKL25Z4 microcontroller. It provides initialization and start functions
 *          for TPM0 and TPM1, as well as an interrupt handler for TPM0. The timers are
 *          used for system timing and can be configured for various modes.
 *
 * Adapted code given by C.Dean
 * @author  Suhas Reddy S
 * @date    1st Dec 2023
 *
 */

#include "timer.h"
#include "MKL25Z4.h"

void Init_TPM(void) {
	//turn on clock to TPM0 and TPM1
	SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK | SIM_SCGC6_TPM1_MASK;

	//set clock source for tpm
	SIM->SOPT2 |= (SIM_SOPT2_TPMSRC(1) | SIM_SOPT2_PLLFLLSEL_MASK);

	// disable TPM0
	TPM0->SC = 0;

	// disable TPM1
	TPM1->SC = 0;

	//load the counter and mod for tmp0 to sample at 96kHz
	TPM0->MOD = 249;

	//load the counter and mod for tmp1 to sample at 48KHz
	TPM1->MOD = 499;

	//set TPM to count up and divide by 2 prescaler and clock mode
	TPM0->SC = (TPM_SC_DMA_MASK | TPM_SC_PS(1));

	//set TPM to count up and divide by 2 prescaler and clock mode
	TPM1->SC = (TPM_SC_DMA_MASK | TPM_SC_PS(1));

	// Enable TPM interrupts for debugging
	TPM0->SC |= TPM_SC_TOIE_MASK;

	// Enable TPM interrupts for debugging
	TPM1->SC |= TPM_SC_TOIE_MASK;

	// Configure NVIC
	NVIC_SetPriority(TPM0_IRQn, 128); // 0, 64, 128 or 192
	NVIC_ClearPendingIRQ(TPM0_IRQn);
	NVIC_EnableIRQ(TPM0_IRQn);

}

void Start_TPM(void) {
    // Enable counter
	TPM0->SC |= TPM_SC_CMOD(1);
	TPM1->SC |= TPM_SC_CMOD(1);
}

void TPM0_IRQHandler() {
	//clear pending IRQ
	NVIC_ClearPendingIRQ(TPM0_IRQn);
	TPM0->SC |= TPM_SC_TOF_MASK;
}
