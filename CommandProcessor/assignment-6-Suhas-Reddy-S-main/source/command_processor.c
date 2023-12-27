/*******************************************************************************
 * Copyright (C) 2023 by Suhas Srinivasa Reddy
 *
 * Redistribution, modification, or use of this software in source or binary
 * forms is permitted as long as the files maintain this copyright. Users are
 * permitted to modify this and use it to learn about the field of embedded
 * software. Suhas Srinivasa Reddy and the University of Colorado are not liable
 * for any misuse of this material.
 ******************************************************************************/

/**
 * @file    command_processor.c
 * @brief   Command processing functions for a serial communication interface.
 *
 * This file contains functions for processing commands received through a serial communication interface.
 * It includes functions for tokenizing commands, converting string representations of numbers,
 * and executing actions based on recognized commands. The supported commands include "ECHO",
 * "LED" for controlling RGB LEDs, and "CLEAR" for clearing the terminal screen.
 *
 * @author  Suhas Reddy S
 * @date    17th nOV 2023
 */

#include "command_processor.h"
#include "stdio.h"
#include "stdint.h"
#include "string.h"
#include "ctype.h"
#include "led.h"
#include "systick.h"

#define WHITE (0xFFFFFFu)
#define ZERO (0)
#define ONE (1)
#define TWO (2)
#define HEXOFFSET (16)
#define DECOFFSET (10)
#define ERROR (-1)
#define MAX_COMMAND_SIZE (50)
#define DELAY_1SEC  (1000)

/**
 * @brief Convert a string to an unsigned 32-bit integer.
 *
 * This function converts a string representing either a decimal or hexadecimal number
 * to an unsigned 32-bit integer.
 *
 * @param str The input string to be converted. This should be a null-terminated string.
 * @return The converted unsigned 32-bit integer.
 *         If the input is a valid number, the converted result is returned.
 *         If the input is not a valid number (contains non-digit characters in a decimal string),
 *         the function returns -1 to indicate an error.
 *
 * @note For hexadecimal strings, the function supports the "0x" or "0X" prefix.
 * @note The function uses the helper function hexCharToUint to convert hexadecimal characters.
 */
uint32_t stringToUint(const char *str) {
	uint32_t result = 0;

	// Check if the string starts with "0x"
	if (str[ZERO] == '0' && (str[ONE] == 'x' || str[ONE] == 'X')) {
		// Hexadecimal string
		str += TWO;

		// Iterate through each character in the string
		while (*str) {
			result = result * HEXOFFSET + hexCharToUint(*str);
			str++;
		}
	} else {
		// Decimal string
		while (*str) {
			if (*str >= '0' && *str <= '9') {
				result = result * DECOFFSET + (*str - '0');
			} else {
				// Non-digit character in decimal string
				return ERROR;
			}
			str++;
		}
	}

	return result;
}

/**
 * @brief Convert a hexadecimal character to its numeric value.
 *
 * This helper function takes a single hexadecimal character as input
 * and returns its numeric value.
 *
 * @param hexChar The hexadecimal character to be converted.
 * @return The numeric value corresponding to the hexadecimal character.
 *         If the input is a valid hexadecimal character ('0' to '9', 'a' to 'f', or 'A' to 'F'),
 *         the function returns its numeric value.
 *         If the input is not a valid hexadecimal character, the function returns -1 to indicate an error.
 */
uint32_t hexCharToUint(char hexChar) {
	if (hexChar >= '0' && hexChar <= '9') {
		return hexChar - '0';
	} else if (hexChar >= 'a' && hexChar <= 'f') {
		return hexChar - 'a' + DECOFFSET;
	} else if (hexChar >= 'A' && hexChar <= 'F') {
		return hexChar - 'A' + DECOFFSET;
	} else {
		// Invalid character
		return ERROR;
	}
}

/**
 * @brief Custom string tokenizer.
 *
 * This function tokenizes a string based on the specified delimiter. It keeps track of the
 * position between calls using a static variable, allowing it to continue tokenizing
 * from where it left off in the previous call.
 *
 * @param str The input string to be tokenized. If NULL, the function continues tokenizing
 *            from the last position stored in the static variable.
 * @param delimiter The delimiter character used for tokenization.
 * @return A pointer to the next token in the string, or NULL if no more tokens are found.
 *
 * @note This function modifies the original string by replacing the delimiter with null characters ('\0').
 *       Ensure that the input string is modifiable or provide a copy if preservation is required.
 * @note If the input string and lastToken are both NULL, the function returns NULL to indicate no more tokens.
 * @note The function is not thread-safe due to the use of a static variable.
 */
char* customStrtok(char *str, const char *delimiter) {
	static char *lastToken = NULL;  // To store the position of the last token

	// If the input string is provided, update the lastToken
	if (str != NULL) {
		lastToken = str;
	} else if (lastToken == NULL) {
		return NULL; // No more tokens if both input string and lastToken are NULL
	}

	// Find the start of the next token (skipping leading delimiters)
	while (*lastToken && strchr(delimiter, *lastToken)) {
		lastToken++;
	}

	// If we reached the end of the string, no more tokens
	if (!*lastToken) {
		lastToken = NULL;
		return NULL;
	}

	// Save the start of the token
	char *tokenStart = lastToken;

	// Find the end of the token
	while (*lastToken && !strchr(delimiter, *lastToken)) {
		lastToken++;
	}

	// If we reached the end of the string, update lastToken to NULL
	if (!*lastToken) {
		lastToken = NULL;
	} else {
		// Replace the delimiter with a null character to terminate the token
		*lastToken = '\0';
		lastToken++;
	}

	return tokenStart;
}

/**
 * @brief Process a command provided as input.
 *
 * This function tokenizes and processes the input command, performing actions based on the command type.
 * Supported commands include "ECHO", "LED", and "CLEAR". The function prints results or error messages
 * depending on the command and its arguments.
 *
 * @param input The input command string to be processed. This should be a null-terminated string.
 *
 * @note The function uses the customStrtok function for tokenization.
 * @note For the LED command, the function expects color values as arguments and sets the RGB LED accordingly.
 * @note For the CLEAR command, the function sends escape sequences to clear the terminal.
 */
void processCommand(char *input) {
	uint8_t valid = ONE;
	// Delimiter used for tokenization (in this case, comma)
	const char delimiter[] = " ,\t\n\r";
	// Tokenize the command
	char *command = customStrtok(input, delimiter), *token, originalcmd[MAX_COMMAND_SIZE];
	strcpy(originalcmd, command);
	if (command == NULL) {
		printf("\n\rUnknown Command(%s)\n\r$$ ", command);
		return;
	}
	// Convert the command to uppercase
	for (int i = 0; i < strlen(command); i++) {
		command[i] = toupper(command[i]);
	}
	printf("\r");    // Return cursor to the left
	if (strcmp(command, "ECHO") == ZERO) {
		// Iterate through the tokens
		token = customStrtok(NULL, delimiter);
		while (token != NULL) {
			// Convert the statement to camel case
			for (int i = 0; i < strlen(token); i++) {
				if (i == 0) {
					token[i] = toupper(token[i]);
				} else {
					token[i] = tolower(token[i]);
				}
			}
			// Print each token with proper space separation
			printf("%s ", token);
			// Get the next token
			token = customStrtok(NULL, delimiter);
		}
	} else if (strcmp(command, "LED") == ZERO) {
		// Iterate through the tokens
		token = customStrtok(NULL, delimiter);
		if (token == NULL) {
			// Log Error if there is no RGB value after LED
			printf("Unknown Command(%s)\n\r$$ ", originalcmd);
			return;
		}
		while (token != NULL) {
			uint32_t color = stringToUint(token);
			if (color > 0xFFFFFF) {
				// Throw error if there RGB value is invalid
				printf("Unknown Command(%s %s)", originalcmd, token);
				token = customStrtok(NULL, delimiter);
				valid = 0;
				continue;
			}
			Set_RGB(color);
			SysTick_Delay(DELAY_1SEC);
			token = customStrtok(NULL, delimiter);
		}

		if (valid) {
			printf("OK");
		}

	} else if (strcmp(command, "CLEAR") == ZERO) {
		// Send escape sequence to clear the terminal
		printf("\033[2J");
		// Move the cursor to the top-left corner
		printf("\033[H");
		printf("Welcome to SerialIO!\n\r$$ ");
		return;

	} else {
		printf("Unknown Command(%s)", originalcmd);
	}
	printf("\n\r$$ ");
}
