/**
 * @file sat_uuid.h
 * @brief UUID (Universally Unique Identifier) generation and manipulation
 * @author Cristiano Silva de Souza
 * @date 2025
 * 
 * This module provides functionality for generating, converting, and comparing
 * UUIDs in both string and binary formats. It supports version 4 (random) UUIDs
 * and allows formatting control for string representations.
 */

#ifndef SAT_UUID_H
#define SAT_UUID_H

#include <stdbool.h>


/** @brief Size of UUID string representation including null terminator */
#define SAT_UUID_STRING_SIZE 37

/** @brief Size of UUID binary representation in bytes */
#define SAT_UUID_BINARY_SIZE 16

/**
 * @brief Binary UUID type (16 bytes)
 * 
 * Array of 16 unsigned chars representing a UUID in binary format.
 */
typedef unsigned char sat_uuid_binary_t [SAT_UUID_BINARY_SIZE];

/**
 * @brief String UUID type (37 characters including null terminator)
 * 
 * Array of chars in the format: xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx
 * Example: "550e8400-e29b-41d4-a716-446655440000"
 */
typedef char sat_uuid_string_t [SAT_UUID_STRING_SIZE];

/**
 * @brief UUID string format options
 * 
 * Defines the case formatting for hexadecimal characters in UUID strings.
 */
typedef enum 
{
    sat_uuid_format_lower_case,  /**< Lowercase hexadecimal (e.g., "a1b2c3") */
    sat_uuid_format_upper_case   /**< Uppercase hexadecimal (e.g., "A1B2C3") */
} sat_uuid_format_t;

/**
 * @brief Generates a new UUID in string format
 * 
 * Creates a new version 4 (random) UUID and formats it as a string with
 * the standard UUID format: xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx
 * 
 * @param[out] uuid_string Buffer to store the generated UUID string (37 bytes)
 * @param[in] format Case formatting for hexadecimal characters
 * @return true on success, false on failure
 * 
 * @note The buffer must be at least SAT_UUID_STRING_SIZE bytes
 * @note The string is null-terminated
 */
bool sat_uuid_generate_string (sat_uuid_string_t uuid_string, sat_uuid_format_t format);

/**
 * @brief Generates a new UUID in binary format
 * 
 * Creates a new version 4 (random) UUID and stores it in binary format.
 * 
 * @param[out] uuid_bin Buffer to store the generated UUID (16 bytes)
 * @return true on success, false on failure
 * 
 * @note The buffer must be at least SAT_UUID_BINARY_SIZE bytes
 */
bool sat_uuid_generate_bin (sat_uuid_binary_t uuid_bin);

/**
 * @brief Converts a UUID from string format to binary format
 * 
 * Parses a UUID string and converts it to binary representation.
 * Accepts both uppercase and lowercase hexadecimal characters.
 * 
 * @param[in] uuid_string UUID string to convert (format: xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx)
 * @param[out] uuid Binary buffer to store the result (16 bytes)
 * @return true on success, false if the string format is invalid
 * 
 * @note The input string must be properly formatted with hyphens
 */
bool sat_uuid_string_to_bin (const sat_uuid_string_t uuid_string, sat_uuid_binary_t uuid);

/**
 * @brief Converts a UUID from binary format to string format
 * 
 * Converts a binary UUID to its string representation with the specified
 * case formatting.
 * 
 * @param[in] uuid_bin Binary UUID to convert (16 bytes)
 * @param[out] uuid_string Buffer to store the string representation (37 bytes)
 * @param[in] format Case formatting for hexadecimal characters
 * @return true on success, false on failure
 * 
 * @note The output buffer must be at least SAT_UUID_STRING_SIZE bytes
 */
bool sat_uuid_bin_to_string (const sat_uuid_binary_t uuid_bin, sat_uuid_string_t uuid_string, sat_uuid_format_t format);

/**
 * @brief Compares two UUIDs in binary format
 * 
 * Performs a byte-by-byte comparison of two binary UUIDs.
 * 
 * @param[in] uuid1 First UUID to compare
 * @param[in] uuid2 Second UUID to compare
 * @return true if UUIDs are equal, false otherwise
 */
bool sat_uuid_compare_bin (const sat_uuid_binary_t uuid1, const sat_uuid_binary_t uuid2);

/**
 * @brief Compares two UUIDs in string format
 * 
 * Compares two UUID strings for equality, normalizing them to the specified
 * format before comparison.
 * 
 * @param[in] uuid1 First UUID string to compare
 * @param[in] uuid2 Second UUID string to compare
 * @param[in] format Format to normalize both strings to before comparison
 * @return true if UUIDs are equal, false otherwise
 * 
 * @note Comparison is case-insensitive but normalized to the specified format
 */
bool sat_uuid_compare_string (const sat_uuid_string_t uuid1, const sat_uuid_string_t uuid2, sat_uuid_format_t format);

#endif /* SAT_UUID_H */
