/*
 * File gives the implementation of the static profiler functions.
 *
 *  Created on: Aug 2, 2023
 *      Author: lpandit
 *
 *  Modified by Suhas Srinivasa Reddy
 *      Date 2nd Nov 2023
 */

#include "MKL25Z4.h"
#include "fsl_debug_console.h"
#define Zero (0)

/* Declarations of the counter and profiling flag */
uint32_t ISHAProcessMessageBlockCount = Zero, ISHAPadMessageCount = Zero, ISHAResetCount = Zero, ISHAInputCount = Zero, ISHAResultCount = Zero;
bool static_profiling_on = false;

/*
 * Turns the static profiler on.
 */
void static_profile_on(void)
{
	static_profiling_on = true;
}

/*
 * Turns the static profiler off.
 */
void static_profile_off(void)
{
	static_profiling_on = true;
}

/*
 *  Prints the summary of the profiling.
 */
void print_static_profiler_summary(void)
{
#ifdef DEBUG
	PRINTF("Static Profile for function call\n\r"
			"ISHAProcessMessageBlock calls: %u\n\r"
			"ISHAPadMessage calls: %u\n\r"
			"ISHAReset calls: %u\n\r"
			"ISHAInput calls: %u\n\r"
			"ISHAResult calls: %u\n\r", ISHAProcessMessageBlockCount, ISHAPadMessageCount, ISHAResetCount, ISHAInputCount, ISHAResultCount);
#endif
}
