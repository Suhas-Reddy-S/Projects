/**
 * @file    ticktime.c
 * @brief   Code to map SysTick timer into msec counter
 * @author  Howdy Pierce, howdy.pierce@colorado.edu
 *
 * Modified by Suhas Srinivasa Reddy
 * 	Date 2nd Nov 2023
 */

#include "MKL25Z4.h"
#include "core_cm0plus.h"
#include "ticktime.h"
#include "pc_profiler.h"

#define SCALE_FACTOR  3
#define Zero (0)

static volatile ticktime_t g_now = Zero;
static ticktime_t g_timer = Zero;

typedef void (*call_back)(uint32_t*);

call_back pc_capture = pc_profile_check;

void init_ticktime(void) {
	// set control & status register to use 48 MHz/16 = 3 MHz
	// clock. Then interrupt 10000 times per second
	SysTick->LOAD = (48000000 / 16) / (SCALE_FACTOR * 10000);
	NVIC_SetPriority(SysTick_IRQn, 3);
	NVIC_ClearPendingIRQ(SysTick_IRQn);
	NVIC_EnableIRQ(SysTick_IRQn);
	SysTick->VAL = Zero;
	SysTick->CTRL &= ~(SysTick_CTRL_CLKSOURCE_Msk);
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;

	g_now = Zero;
	g_timer = Zero;
}

void SysTick_Handler(void) {
#ifdef DEBUG
	register uint32_t *sp;
	asm("mov %0, sp" : "=r"(sp));  // Retrieve Stack Pointer
	pc_capture(sp);                // Call PC capture
#endif
	g_now++;
}

ticktime_t now(void) {
	return g_now / SCALE_FACTOR;
}

void reset_timer(void) {
	g_timer = g_now;
}

ticktime_t get_timer(void) {
	return (g_now - g_timer) / SCALE_FACTOR;
}
