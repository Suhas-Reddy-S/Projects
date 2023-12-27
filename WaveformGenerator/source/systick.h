/**
 * @file    systick.h
 * @brief   Header file for SysTick timer control functions and declarations.
 *
 * This header file provides declarations and function prototypes for controlling Systick timer.
 *
 * @author  Suhas Reddy S
 * @date    16th Oct 2023
 */

#ifndef _SYSTICK_H_
#define _SYSTICK_H_

#include <stdbool.h>

/**
 * @brief Initialize the SysTick timer.
 *
 * This function is responsible for initializing the SysTick timer. It configures
 * the timer to generate periodic interrupts, enabling precise time-keeping and delays.
 */
void Init_SysTick(void);

#endif /* _SYSTICK_H_ */
