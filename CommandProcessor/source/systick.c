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

volatile uint32_t ticks, system_ticks = 0;
bool emergency_flag = false;

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
	SysTick->LOAD = (48000UL / 16);  // Set the reload value for a 1ms period
	SysTick->VAL = 0;               // Clear the current value
	SysTick->CTRL = SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk; // Enable timer and interrupts
	NVIC_SetPriority(SysTick_IRQn, 3);  // Set the interrupt priority
	NVIC_ClearPendingIRQ(SysTick_IRQn);  // Clear any pending interrupts
	NVIC_EnableIRQ(SysTick_IRQn);       // Enable the SysTick timer interrupt
}

/**
 * @brief Generate a delay using the SysTick timer.
 *
 * This function generates a delay by setting the 'ticks' variable and waiting
 * for it to reach zero. The delay is in milliseconds.
 *
 * @param ms The delay time in milliseconds.
 */
void SysTick_Delay(uint32_t ms) {
	ticks = ms;
	while (ticks > 0) {
		// Wait for 'ticks' to reach zero
	}
}

/**
 * @brief SysTick timer interrupt handler.
 *
 * This interrupt handler decrements the 'ticks' variable and increments the
 * 'system_ticks' variable on each SysTick timer interrupt.
 */
void SysTick_Handler() {
	ticks--;
	system_ticks++;
	// Add any additional logic that may be required
}
