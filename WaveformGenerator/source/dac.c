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
 * @file    dac.c
 * @brief   Initializes DAC interface.
 *
 * @author  Suhas Reddy S
 * @date    1st Dec 2023
 *
 */

#include "MKL25Z4.h"
#include "dac.h"

#define DAC_POS (30)      // Used to set 30th pin on port E

void Init_DAC(void) {
	// Enable Clock to DAC) and PortE
	SIM->SCGC6 |= (1UL << SIM_SCGC6_DAC0_SHIFT);
	SIM->SCGC5 |= (1UL << SIM_SCGC5_PORTE_SHIFT);

	PORTE->PCR[DAC_POS] &= ~(PORT_PCR_MUX(7));	// Select analog output via PORTE pin 30

	// Disable buffer mode
	DAC0->C1 = 0;
	DAC0->C2 = 0;

	// Enable DAC, select VDDA as reference voltage
	DAC0->C0 = (1 << DAC_C0_DACEN_SHIFT) | (1 << DAC_C0_DACRFS_SHIFT);

}
