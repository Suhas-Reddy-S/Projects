#ifndef COMMAND_PROCESSOR_H
#define COMMAND_PROCESSOR_H

#include <stdint.h>

/**
 * @brief Process a command provided as input.
 *
 * This function processes the input command and performs the necessary actions.
 *
 * @param input The input command string.
 */
void processCommand(char *input);

/**
 * @brief Convert a hexadecimal character to its numeric value.
 *
 * This function takes a hexadecimal character as input and returns its numeric value.
 *
 * @param hexChar The hexadecimal character to be converted.
 * @return The numeric value corresponding to the hexadecimal character.
 */
uint32_t hexCharToUint(char hexChar);

/**
 * @brief Convert a string to an unsigned 32-bit integer.
 *
 * This function converts a string representing either a decimal or hexadecimal number
 * to an unsigned 32-bit integer.
 *
 * @param str The input string to be converted.
 * @return The converted unsigned 32-bit integer.
 */
uint32_t stringToUint(const char *str);

#endif // COMMAND_PROCESSOR_H
