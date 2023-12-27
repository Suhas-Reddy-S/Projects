/*
 * main.c - application entry point
 *
 * Author Howdy Pierce, howdy.pierce@colorado.edu
 */
#include "sysclock.h"
#include "test_cbfifo.h"
#include "uart.h"
#include "cbfifo.h"
#include "MKL25Z4.h"
#include "stdio.h"
#include "led.h"
#include "command_processor.h"
#include "systick.h"

int main(void) {
	sysclock_init();
	Init_LEDS();
	Init_UART0();
	Init_SysTick();

	char inputBuffer[127], c, clear[] = "clear";
	int inputIndex = 0;

	processCommand(clear);     // Clear The Terminal Window
	// enter infinite loop
	while (1) {
		c = getchar();
		if (c == 13) {
			printf("\n\r");    // Move the cursor to a new line
			// End of command, process it
			inputBuffer[inputIndex] = '\0';
			processCommand(inputBuffer);
			// Reset for the next command
			inputIndex = 0;
		} else if (c == '\b') {
			// Handle Backspace
			inputIndex--;
			inputBuffer[inputIndex] = ' ';
		} else {
			// Store the character in the input buffer
			inputBuffer[inputIndex] = c;
			inputIndex++;
		}
	}
	return 0;
}

