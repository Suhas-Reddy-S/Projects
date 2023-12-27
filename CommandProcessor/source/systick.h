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

/**
 * @brief Generate a time delay using the SysTick timer.
 *
 * This function generates a time delay by setting the 'ticks' variable and waiting
 * for it to reach zero. The provided delay is in milliseconds, allowing for precise
 * timing in applications.
 *
 * @param ms The time delay in milliseconds to be generated using the SysTick timer.
 */
void SysTick_Delay(uint32_t ms);

#endif /* _SYSTICK_H_ */
