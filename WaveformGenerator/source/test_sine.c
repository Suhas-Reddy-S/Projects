/*
 * test_sine.c: File with test for your sine function.
 *
 *  Created on: Apr 25, 2022
 *      Author: lpandit
 */
#include <stdio.h>
#include <math.h>

#include "fp_trig.h"
#include "test_sine.h"
#include "fp_trig_sin.h"

/*
 * Test the sine function.
 *
 * Your sin function should accept inputs in the range [INT_MIN, INT_MAX] and
 * produce outputs in the range [-TRIG_SCALE_FACTOR, TRIG_SCALE_FACTOR].
 *
 * Your code needs to provide sine.h which should declare TWO_PI and TRIG_SCALE_FACTOR.
 *
 * Ensure that max_err is <= 2.0 and sum_sq error is <= 12000.
 */

void test_sin() {
	double act_sin;
	double exp_sin;
	double test_sin;
	double err;
	double sum_sq = 0;
	double max_err = 0;
	double test_err;
	double test_sum_sq = 0;
	double test_max_err = 0;

	for (int i = -TWO_PI; i <= TWO_PI; i++) {
		exp_sin = sin((double) i / TRIG_SCALE_FACTOR) * TRIG_SCALE_FACTOR;
		act_sin = fp_sin(i);
		test_sin = fp_sin_test(i);

		err = act_sin - exp_sin;
		test_err = test_sin - exp_sin;
		if (err < 0)
			err = -err;

		if (err > max_err)
			max_err = err;
		sum_sq += err * err;

		if (test_err < 0)
			test_err = -test_err;

		if (test_err > test_max_err)
			test_max_err = test_err;
		test_sum_sq += test_err * test_err;
	}

	printf("\n\rGiven Sine function: max_err=%f  sum_sq=%f\n\r", max_err, sum_sq);
	printf("\n\rImplemented Sine function: max_err=%f  sum_sq=%f\n\r", test_max_err, test_sum_sq);

	if (max_err > 2.0 || sum_sq > 12000) {
		printf("Error: Do not proceed. Your sine function needs work\n\r");
	}
}
