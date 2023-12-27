/*******************************************************************************
 * Copyright (C) 2023 by Suhas Srinivasa Reddy
 *
 * Redistribution, modification or use of this software in source or binary
 * forms is permitted as long as the files maintain this copyright. Users are
 * permitted to modify this and use it to learn about the field of embedded
 * software. Suhas Srinivasa Reddy and the University of Colorado are not liable
 * for any misuse of this material.
 * ****************************************************************************/

/**
 * @file    cbfifo.c
 * @brief
 *
 *
 * @author  Suhas Srinivasa Reddy
 * @date    19th sept 2023
 *
 */
#include "cbfifo.h"
#include <MKL25Z4.h>

Buffer tx_buffer;
Buffer rx_buffer; // Static initialization of struct buffer
uint32_t masking_state;

/**
 * @brief      { This function enqueue nbytes of data into the circular buffer from the source.}
 *
 * @param[in]  buf        Input data to the buffer
 * @param[in]  nbyte      Size of the buffer
 *
 * @return     { Returns number of bytes enqueued or 0 if no byte is added.}
 */
size_t cbfifo_enqueue(Buffer *buffer, void *buf, size_t nbyte) {
	// protect q->Size++ operation from preemption
	// save current masking state
	masking_state = __get_PRIMASK();
	// disable interrupts
	__disable_irq();
	// update variable

	if (!buf || nbyte == 0 || buffer->elements == cbfifo_capacity()) {
		return ZERO; // Nothing to enqueue or buffer is full
	}

	// Calculates the number of bytes that can be enqueued
	size_t bytes_to_enqueue =
			(nbyte <= (cbfifo_capacity() - buffer->elements)) ?
					nbyte : (cbfifo_capacity() - buffer->elements);

	char *src = (char*) buf;

	// Writing bytes from the source to buffer array
	for (size_t i = 0; i < bytes_to_enqueue; i++) {
		buffer->buffer_array[buffer->tail] = src[i];
		buffer->tail++;

		if (buffer->tail == cbfifo_capacity()) {
			buffer->tail = 0; // Tail index warp-around
		}

		buffer->elements++; // Updating number of elements in the buffer
	}

	// restore  interrupt masking state
	__set_PRIMASK(masking_state);
	return bytes_to_enqueue;
}

/**
 * @brief      { This function dequeue nbytes of data from the circular buffer to the desination.}
 *
 * @param[in]  buf        Output data from the buffer
 * @param[in]  nbyte      Size of the requested output data
 *
 * @return     { Returns number of bytes dequeued 0 if no byte is removed.}
 */
size_t cbfifo_dequeue(Buffer *buffer, void *buf, size_t nbyte) {
	// protect q->Size++ operation from preemption
	// save current masking state
	masking_state = __get_PRIMASK();
	// disable interrupts
	__disable_irq();
	// update variable
	if (!buf || nbyte == 0 || buffer->elements == 0) {
		return ZERO; // Nothing to dequeue
	}

	// Calculates the number of bytes that can be enqueued
	size_t bytes_to_dequeue =
			(nbyte <= buffer->elements) ? nbyte : buffer->elements;

	char *dest = (char*) buf;

	// Reading bytes from the buffer to destination
	for (size_t i = 0; i < bytes_to_dequeue; i++) {
		dest[i] = buffer->buffer_array[buffer->head];
		buffer->head++;

		if (buffer->head == cbfifo_capacity()) {
			buffer->head = 0; // Head index warp-around
		}

		buffer->elements--;
	}
	// restore  interrupt masking state
	__set_PRIMASK(masking_state);

	return bytes_to_dequeue;
}

/**
 * @brief      { This function returns number of elements in the buffer.}
 *
 * @return     { Returns number of elements in the buffer.}
 */
size_t cbfifo_length(Buffer *buffer) {
	return buffer->elements;
}

/**
 * @brief      { This function returns the total capacity of the buffer.}
 *
 * @return     { Returns the total capacity of the buffer.}
 */
size_t cbfifo_capacity() {
	return BUFFER_SIZE;
}

/**
 * @brief      { This function resets the buffer.}
 */
void cbfifo_reset(Buffer *buffer) {
	buffer->head = 0;
	buffer->tail = 0;
	buffer->elements = 0;
}

void cbfifo_clearlastele(Buffer *buffer) {
	// protect q->Size++ operation from preemption
	// save current masking state
	masking_state = __get_PRIMASK();
	// disable interrupts
	__disable_irq();
	// update variable
	buffer->tail--;
	// restore  interrupt masking state
	__set_PRIMASK(masking_state);
}
