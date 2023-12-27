/**
 * @file    uart.c
 * @brief   UART (Universal Asynchronous Receiver-Transmitter) communication functions.
 *
 * This file contains functions for initializing the UART module, transmitting strings,
 * and managing UART communication. It includes functions for initializing UART0,
 * sending null-terminated strings, and transmitting arrays of bytes.
 *
 * @author  Suhas Reddy S
 * @date    17th November 2023
 */

#include <MKL25Z4.h>
#include "cbfifo.h"
#include "uart.h"
#include "sysclock.h"
#include "stdio.h"

#define BAUD_RATE 	(38400)
#define UART_OVERSAMPLE_RATE 	(16)
#define DATA_BITS  (1)     // 1 for 8 bits and 0 for 9 bits
#define STOP_BITS (1)      // 0 for 1 stop bit and 1 for 2 stop bits
#define PARITY_ENABLE (1)  // 1 to enable parity
#define PARITY_TYPE (1)    // 1 for odd and 0 for even
#define SBR_MSBYTE (8)
#define ONE (1)
#define TWO (2)

// Redirecting Printf to Write to UART console
int __sys_write(int handle, char *buffer, int size) {
	int count = size;
	while (count--) {
		UART0_Transmit((uint8_t*) buffer++);
	}
	return size;
}

// Redirecting getchar to read from UART console
int __sys_readc(void) {
	char ch;
	// Wait until a character is available
	while (cbfifo_length(&rx_buffer) == ZERO)
		;
	cbfifo_dequeue(&rx_buffer, &ch, ONE);
	return ch;
}

/**
 * @brief   Initialize UART0 for serial communication.
 *
 * This function configures and initializes the UART0 module for serial communication.
 * It sets up the necessary clock gating, pins for Rx and Tx, baud rate, data format,
 * and enables the UART receiver and transmitter. Additionally, it configures interrupts
 * for UART receive and clears any error flags.
 *
 * @note    The function uses a 24 MHz clock source for UART0 and supports an 8-bit data format,
 *          one stop bit, and optional parity.
 * @note    The baud rate is calculated based on the system clock frequency and the specified
 *          baud rate constant.
 * @note    The function enables UART interrupts for receive and initializes the NVIC.
 *
 * Author Prof. Dean
 *
 * Modified by Suhas Srinivasa Reddy
 *
 */
void Init_UART0() {
	uint16_t sbr;

	// Enable clock gating for UART0 and Port A
	SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;

	// Make sure transmitter and receiver are disabled before init
	UART0->C2 &= ~UART0_C2_TE_MASK & ~UART0_C2_RE_MASK;

	// Set UART clock to 24 MHz clock
	SIM->SOPT2 |= SIM_SOPT2_UART0SRC(1);

	// Set pins to UART0 Rx and Tx
	PORTA->PCR[1] |= PORT_PCR_ISF_MASK | PORT_PCR_MUX(TWO); // Rx
	PORTA->PCR[2] |= PORT_PCR_ISF_MASK | PORT_PCR_MUX(TWO); // Tx

	// Set baud rate and oversampling ratio
	sbr =
			(uint16_t) ((SYSCLOCK_FREQUENCY)
					/ (BAUD_RATE * UART_OVERSAMPLE_RATE));
	UART0->BDH &= ~UART0_BDH_SBR_MASK;
	UART0->BDH |= UART0_BDH_SBR(sbr >> SBR_MSBYTE);
	UART0->BDL = UART0_BDL_SBR(sbr);
	UART0->C4 |= UART0_C4_OSR(UART_OVERSAMPLE_RATE-ONE);

	// Disable interrupts for RX active edge and LIN break detect, select two stop bit
	UART0->BDH |=
	UART0_BDH_RXEDGIE(ZERO) | UART0_BDH_SBNS(STOP_BITS) | UART0_BDH_LBKDIE(ZERO);

	// Don't enable loopback mode, use 8 data bit mode, use parity and odd parity type
	UART0->C1 = UART0_C1_LOOPS(
			ZERO) | UART0_C1_M(DATA_BITS) | UART0_C1_PE(PARITY_ENABLE) | UART0_C1_PT(PARITY_TYPE);
	// Don't invert transmit data, don't enable interrupts for errors
	UART0->C3 = UART0_C3_TXINV(ZERO) | UART0_C3_ORIE(ZERO)| UART0_C3_NEIE(ZERO)
	| UART0_C3_FEIE(ZERO) | UART0_C3_PEIE(ZERO);

	// Clear error flags
	UART0->S1 = UART0_S1_OR(
			ONE) | UART0_S1_NF(ONE) | UART0_S1_FE(ONE) | UART0_S1_PF(ONE);

	// Send LSB first, do not invert received data
	UART0->S2 = UART0_S2_MSBF(ZERO) | UART0_S2_RXINV(ZERO);
	// Enable interrupts. Listing 8.11 on p. 234
	NVIC_SetPriority(UART0_IRQn, TWO); // 0, 1, 2, or 3
	NVIC_ClearPendingIRQ(UART0_IRQn);
	NVIC_EnableIRQ(UART0_IRQn);

	// Enable receive interrupts but not transmit interrupts yet
	UART0->C2 |= UART_C2_RIE(ONE);

	// Enable UART receiver and transmitter
	UART0->C2 |= UART0_C2_RE(ONE) | UART0_C2_TE(ONE);

}

// UART0 IRQ Handler.
void UART0_IRQHandler(void) {
	// Check for errors
	if (UART0->S1 & (UART0_S1_OR_MASK | UART0_S1_NF_MASK |
	UART0_S1_FE_MASK | UART0_S1_PF_MASK)) {
		// Handle error conditions here
		// For example, you might clear the error flags
		UART0->S1 |= UART0_S1_OR_MASK | UART0_S1_NF_MASK |
		UART0_S1_FE_MASK | UART0_S1_PF_MASK;
		// You might also reset or handle other aspects of your application
	}

	// Check if the interrupt is due to received data
	if (UART0->S1 & UART0_S1_RDRF_MASK) {
		char ch;
		// received a character
		ch = UART0->D;
		if (ch == '\b') {
			printf("\b ");
		}
		UART0_Transmit((uint8_t*) &ch);
		if (cbfifo_length(&rx_buffer) < BUFFER_SIZE) {
			cbfifo_enqueue(&rx_buffer, &ch, ONE);
		}
	}

	// Check if the interrupt is due to the transmitter being ready
	if (UART0->S1 & UART0_S1_TDRE_MASK) {
		char ch;
		// can send another character
		if (cbfifo_length(&tx_buffer) != ZERO) {
			cbfifo_dequeue(&tx_buffer, &ch, ONE);
			UART0->D = ch;
		} else {
			// queue is empty, disable transmitter interrupt
			UART0->C2 &= ~UART0_C2_TIE_MASK;
		}
	}
}

/**
 * @brief   Transmit an array of bytes over UART0.
 *
 * This function enqueues an array of bytes for transmission over UART0.
 * It waits for space to become available in the circular buffer before enqueuing the data.
 * Once the data is enqueued, it enables the UART transmitter interrupt to start the transmission process.
 *
 * @param data The array of bytes to be transmitted.
 *
 * @note    The function uses a circular buffer (`cbfifo`) for managing the transmit data.
 * @note    It waits for space to open up in the circular buffer before enqueuing the data.
 * @note    The UART transmitter interrupt is enabled to initiate the transmission process.
 */
void UART0_Transmit(uint8_t *data) {
	while (cbfifo_length(&tx_buffer) == BUFFER_SIZE)
		; // wait for space to open up
	cbfifo_enqueue(&tx_buffer, data, ONE);
	// Enable transmitter interrupt
	UART0->C2 |= UART_C2_TIE(ONE);
}
