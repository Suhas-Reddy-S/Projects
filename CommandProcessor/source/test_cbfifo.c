#include "cbfifo.h"
#include "test_cbfifo.h"
#include "string.h"
#include "stdio.h"

#define BUFFER_SIZE 127

int cbfifo_tests_passed = 0;
int cbfifo_tests_failed = 0;

// Macro to Assert and update pass and fail values
#define TEST_ASSERT(expression)                                            \
    do                                                                     \
    {                                                                      \
        if (expression)                                                    \
        {                                                                  \
            cbfifo_tests_passed++;                                         \
        }                                                                  \
        else                                                               \
        {                                                                  \
            cbfifo_tests_failed++;                                         \
            printf("Test failed at line %d: %s\n\r", __LINE__, #expression); \
        }                                                                  \
    } while (0)

void test_cbfifo_capacity() {
	TEST_ASSERT(cbfifo_capacity() == BUFFER_SIZE);
}

void test_rx_cbfifo_enqueue() {
	char data[] = "hello";

	// Happy path
	TEST_ASSERT(cbfifo_enqueue(&rx_buffer, data, strlen(data)) == strlen(data));

	// Enqueue to a full buffer
	char overflow[BUFFER_SIZE + 5];
	memset(overflow, 'a', sizeof(overflow));
	TEST_ASSERT(
			cbfifo_enqueue(&rx_buffer, overflow, sizeof(overflow))
					== cbfifo_capacity() - strlen(data));

	// Enqueue with NULL data
	TEST_ASSERT(cbfifo_enqueue(&rx_buffer, NULL, 5) == 0);

	// Enqueue with nbyte as 0
	TEST_ASSERT(cbfifo_enqueue(&rx_buffer, data, 0) == 0);
}

void test_rx_cbfifo_dequeue() {
	char dest[10];

	cbfifo_reset(&rx_buffer); // Reset buffer for testing

	char data[] = "world";

	cbfifo_enqueue(&rx_buffer, data, strlen(data));

	// Happy path
	TEST_ASSERT(cbfifo_dequeue(&rx_buffer, dest, 3) == 3);

	// Dequeue more than enqueued
	TEST_ASSERT(cbfifo_dequeue(&rx_buffer, dest, 10) == 2); // Only 'l' and 'd' are left

	// Dequeue from an empty buffer
	TEST_ASSERT(cbfifo_dequeue(&rx_buffer, dest, 5) == 0);

	// Dequeue with NULL destination
	TEST_ASSERT(cbfifo_dequeue(&rx_buffer, NULL, 5) == 0);

	// Dequeue with nbyte as 0
	TEST_ASSERT(cbfifo_dequeue(&rx_buffer, dest, 0) == 0);
}

void test_rx_cbfifo_length() {
	cbfifo_reset(&rx_buffer); // Reset buffer for testing

	char data[] = "test";
	cbfifo_enqueue(&rx_buffer, data, strlen(data));
	TEST_ASSERT(cbfifo_length(&rx_buffer) == strlen(data));
}

void test_rx_cbfifo_reset() {
	cbfifo_reset(&rx_buffer); // Reset buffer for testing

	char data[] = "reset";
	cbfifo_enqueue(&rx_buffer, data, strlen(data));
	cbfifo_reset(&rx_buffer);
	TEST_ASSERT(cbfifo_length(&rx_buffer) == 0);
}

void test_tx_cbfifo_enqueue() {
	char data[] = "hello";

	// Happy path
	TEST_ASSERT(cbfifo_enqueue(&tx_buffer, data, strlen(data)) == strlen(data));

	// Enqueue to a full buffer
	char overflow[BUFFER_SIZE + 5];
	memset(overflow, 'a', sizeof(overflow));
	TEST_ASSERT(
			cbfifo_enqueue(&tx_buffer, overflow, sizeof(overflow))
					== cbfifo_capacity() - strlen(data));

	// Enqueue with NULL data
	TEST_ASSERT(cbfifo_enqueue(&tx_buffer, NULL, 5) == 0);

	// Enqueue with nbyte as 0
	TEST_ASSERT(cbfifo_enqueue(&tx_buffer, data, 0) == 0);
}

void test_tx_cbfifo_dequeue() {
	char dest[10];

	cbfifo_reset(&tx_buffer); // Reset buffer for testing

	char data[] = "world";

	cbfifo_enqueue(&tx_buffer, data, strlen(data));

	// Happy path
	TEST_ASSERT(cbfifo_dequeue(&tx_buffer, dest, 3) == 3);

	// Dequeue more than enqueued
	TEST_ASSERT(cbfifo_dequeue(&tx_buffer, dest, 10) == 2); // Only 'l' and 'd' are left

	// Dequeue from an empty buffer
	TEST_ASSERT(cbfifo_dequeue(&tx_buffer, dest, 5) == 0);

	// Dequeue with NULL destination
	TEST_ASSERT(cbfifo_dequeue(&tx_buffer, NULL, 5) == 0);

	// Dequeue with nbyte as 0
	TEST_ASSERT(cbfifo_dequeue(&tx_buffer, dest, 0) == 0);
}

void test_tx_cbfifo_length() {
	cbfifo_reset(&tx_buffer); // Reset buffer for testing

	char data[] = "test";
	cbfifo_enqueue(&tx_buffer, data, strlen(data));
	TEST_ASSERT(cbfifo_length(&tx_buffer) == strlen(data));
}

void test_tx_cbfifo_reset() {
	cbfifo_reset(&rx_buffer); // Reset buffer for testing

	char data[] = "reset";
	cbfifo_enqueue(&tx_buffer, data, strlen(data));
	cbfifo_reset(&tx_buffer);
	TEST_ASSERT(cbfifo_length(&tx_buffer) == 0);
}

void run_cbfifo_tests() {
	printf("Running tests for CBFIFO...\n\r");

	// Call each individual test function here
	test_cbfifo_capacity();
	test_rx_cbfifo_enqueue();
	test_rx_cbfifo_dequeue();
	test_rx_cbfifo_length();
	test_rx_cbfifo_reset();
	test_tx_cbfifo_enqueue();
	test_tx_cbfifo_dequeue();
	test_tx_cbfifo_length();
	test_tx_cbfifo_reset();

	printf("Tests passed: %d\n\r", cbfifo_tests_passed);
	printf("Tests failed: %d\n\r", cbfifo_tests_failed);
}
