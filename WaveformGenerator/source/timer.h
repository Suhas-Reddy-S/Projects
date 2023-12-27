#ifndef TIMERS_H
#define TIMERS_H

#include "MKL25Z4.h"

/**
 * @file    timers.h
 * @brief   Header file containing function declarations for configuring and using Timers/PWM Modules (TPM).
 * @author  Suhas Reddy S
 * @date    1st Dec 2023
 */

/**
 * @brief   Initializes Timers/PWM Modules (TPM0 and TPM1).
 * @return  None
 *
 * This function sets up the necessary configurations for TPM0 and TPM1,
 * including clock sources, counters, and prescalers. It also configures
 * TPM0 for overflow interrupts.
 */
void Init_TPM(void);

/**
 * @brief   Starts the counters for Timers/PWM Modules (TPM0 and TPM1).
 * @return  None
 *
 * This function enables the counters for TPM0 and TPM1, allowing them
 * to start counting based on the configured settings.
 */
void Start_TPM(void);

#endif  // TIMERS_H


