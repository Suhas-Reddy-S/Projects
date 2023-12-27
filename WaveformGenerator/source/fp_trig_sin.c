#include "fp_trig_sin.h"
#include "fp_trig.h"

#define THREE_FACT 6
#define FIVE_FACT 120
#define SEVEN_FACT 5040
#define NINE_FACT 362880

// Function to calculate the sine using a 9-term Taylor series
int16_t fp_sin_test(int16_t i) {
	int16_t y = i;

	if ((y > PI) && (y <= TWO_PI)) {
		y = y - TWO_PI;
	}

	if ((y < -PI) && (y >= -TWO_PI)) {
		y = y + TWO_PI;
	}

	if ((y > HALF_PI) && (y <= (HALF_PI + PI))) {
		y = PI - y;
	}

	double x = (double) y / TRIG_SCALE_FACTOR;
	double result = 0.0, x2, x3, x5, x7, x9;

	x2 = x * x;
	x3 = x2 * x;
	x5 = x3 * x2;
	x7 = x5 * x2;
	x9 = x7 * x2;

	result = x - x3 / THREE_FACT + x5 / FIVE_FACT - x7 / SEVEN_FACT
			+ x9 / NINE_FACT;

	return (int16_t) (result * TRIG_SCALE_FACTOR);
}
