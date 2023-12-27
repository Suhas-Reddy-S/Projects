/*
 * profiler.c
 *
 *  Created on: Aug 2, 2023
 *      Author: lpandit
 *
 *  Modified by Suhas Srinivasa Reddy
 *      Date 2nd Nov 2023
 */

#include "MKL25Z4.h"
#include "fsl_debug_console.h"
#include "pc_profiler.h"
#include "isha.h"

#define EIGHT (8)
#define Zero (0)

uint32_t PC_ISHAProcessMessageBlockCount = Zero, PC_ISHAPadMessageCount = Zero,
		PC_ISHAResetCount = Zero, PC_ISHAInputCount = Zero, PC_ISHAResultCount = Zero,
		start, end;
bool pc_profiling_on;
uint32_t *add;

/*
 * Checks the program counter (PC) to determine which function is being called
 * and increments the respective function's call count.
 *
 * Parameters:
 *   pc  Pointer to the program counter (PC)
 */
void pc_profile_check(uint32_t *pc) {
	if (!pc_profiling_on) {
		return;
	}

	add = (uint32_t*) *(pc+EIGHT);     // Offset Stack Pointer by eight to get Saved Program counter

	GetFunctionAddress("ISHAProcessMessageBlock", &start, &end);
	if ((uint32_t) add >= start && (uint32_t) add <= end) {
		PC_ISHAProcessMessageBlockCount++;
		return;
	}
	GetFunctionAddress("ISHAPadMessage", &start, &end);
	if ((uint32_t) add >= start && (uint32_t) add <= end) {
		PC_ISHAPadMessageCount++;
		return;
	}

	GetFunctionAddress("ISHAReset", &start, &end);
	if ((uint32_t) add >= start && (uint32_t) add <= end) {
		PC_ISHAResetCount++;
		return;
	}

	GetFunctionAddress("ISHAInput", &start, &end);
	if ((uint32_t) add >= start && (uint32_t) add <= end) {
		PC_ISHAInputCount++;
		return;
	}

	GetFunctionAddress("ISHAResult", &start, &end);
	if ((uint32_t) add >= start && (uint32_t) add <= end) {
		PC_ISHAResultCount++;
	}

}

/*
 * Turn the profiler on for call counting.
 */
void pc_profile_on(void) {
	pc_profiling_on = true;
}

/*
 * Turn the profiler off for call counting.
 */
void pc_profile_off(void) {
	pc_profiling_on = false;
}

/*
 * Print a summary of profiler calls for following functions.
 *
 */
void print_pc_profiler_summary(void) {
#ifdef DEBUG
	PRINTF("PC Profile for function call\n\r"
			"ISHAProcessMessageBlock calls: %u\n\r"
			"ISHAPadMessage calls: %u\n\r"
			"ISHAReset calls: %u\n\r"
			"ISHAInput calls: %u\n\r"
			"ISHAResult calls: %u\n\r", PC_ISHAProcessMessageBlockCount,
			PC_ISHAPadMessageCount, PC_ISHAResetCount, PC_ISHAInputCount,
			PC_ISHAResultCount);
#endif
}
