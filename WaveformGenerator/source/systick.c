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
 * @file    systick.c
 * @brief   SysTick timer functions for time-keeping and delays.
 *
 * This file contains functions for initializing the SysTick timer, generating
 * delays, and handling SysTick timer interrupts. The SysTick timer is configured
 * to generate periodic interrupts at a specified interval, allowing for precise
 * timing and delays.
 *
 * @author  Suhas Srinivasa Reddy
 * @date    16th Oct 2023
 */

#include <MKL25Z4.h>
#include "systick.h"
#include "dma.h"

#define SYSTICK_LOAD (6000000UL)
/**
 * @brief Initialize the SysTick timer.
 *
 * This function configures the SysTick timer to generate periodic interrupts.
 * It sets the load value, enables the timer, and configures the interrupt priority.
 *
 * @note Original Author: Dean
 * @note Modifications by: Suhas Srinivasa Reddy
 */
void Init_SysTick(void) {
	SysTick->LOAD = SYSTICK_LOAD;  // Set the reload value for a 1ms period
	SysTick->VAL = 0;               // Clear the current value
	SysTick->CTRL = SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk; // Enable timer and interrupts
	NVIC_SetPriority(SysTick_IRQn, 1);  // Set the interrupt priority
	NVIC_ClearPendingIRQ(SysTick_IRQn);  // Clear any pending interrupts
	NVIC_EnableIRQ(SysTick_IRQn);       // Enable the SysTick timer interrupt
}

/**
 * @brief   SysTick timer interrupt handler.
 * @details This interrupt handler calls the 'CallBack_Function()' to update the
 * 			source for DMA, generating different waveforms during the SysTick
 * 			timer interrupt.
 */
void SysTick_Handler() {
	// Call function to update the source to DMA to generate differen wave forms
	CallBack_Function();
}
