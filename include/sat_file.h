/**
 * @file sat_file.h
 * @brief File I/O Operations Module
 * @version 1.0
 * @date 2025
 * 
 * @description
 * This module provides file I/O operations for the SAT framework.
 * It offers functions for opening, reading, writing, and managing files.
 * 
 * @features
 * - File operations with different access modes
 * - Data reading and writing
 * - File size and existence checking
 * - Convenient file reading functions
 * 
 * @usage_example
 * @code
 * sat_file_t file;
 * if (sat_file_open(&file, "data.txt", sat_file_mode_read)) {
 *     char buffer[1024];
 *     if (sat_file_read(&file, buffer, sizeof(buffer))) {
 *         // Process file data
 *     }
 *     sat_file_close(&file);
 * }
 * @endcode
 * 
 * @author SAT Framework Team
 * @copyright Copyright (c) 2025 SAT Framework
 */

#ifndef SAT_FILE_H
#define SAT_FILE_H

#include <stdbool.h>
#include <stdint.h>
#include <sat_status.h>

/**
 * @brief File access modes
 */
typedef enum 
{
    sat_file_mode_read,    /**< Read mode */
    sat_file_mode_write,   /**< Write mode */
    sat_file_mode_append   /**< Append mode */
} sat_file_mode_t;

/**
 * @brief File handle structure
 */
typedef struct 
{
    void *handle;  /**< File handle */
} sat_file_t;

/**
 * @brief Open a file
 * 
 * @param object File object to initialize
 * @param filename Path to the file
 * @param mode Access mode
 * @return true if successful, false otherwise
 */
bool sat_file_open (sat_file_t *object, const char *filename, sat_file_mode_t mode);

/**
 * @brief Read data from file
 * 
 * @param object File object
 * @param buffer Buffer to store read data
 * @param size Number of bytes to read
 * @return true if successful, false otherwise
 */
bool sat_file_read (sat_file_t *object, void *buffer, uint32_t size);

/**
 * @brief Read a line from file
 * 
 * @param object File object
 * @param buffer Buffer to store the line
 * @param size Size of the buffer
 * @return true if successful, false otherwise
 */
bool sat_file_readline (sat_file_t *object, void *buffer, uint32_t size);

/**
 * @brief Write data to file
 * 
 * @param object File object
 * @param buffer Data to write
 * @param size Number of bytes to write
 * @return true if successful, false otherwise
 */
bool sat_file_write (sat_file_t *object, const void *buffer, uint32_t size);

/**
 * @brief Get file size
 * 
 * @param object File object
 * @return File size in bytes
 */
uint32_t sat_file_get_size (sat_file_t *object);

/**
 * @brief Check if file exists
 * 
 * @param filename Path to the file
 * @return true if file exists, false otherwise
 */
bool sat_file_exists (const char *filename);

/**
 * @brief Read entire file into buffer
 * 
 * @param filename Path to the file
 * @param buffer Pointer to store allocated buffer
 * @param size Maximum size to read (0 for entire file)
 * @return true if successful, false otherwise
 * @note Caller must free the allocated buffer
 */
bool sat_file_read_to_buffer (const char *filename, void **buffer, uint32_t size);



/**
 * @brief Close file
 * 
 * @param object File object to close
 * @return true if successful, false otherwise
 */
bool sat_file_close (sat_file_t *object);

sat_status_t sat_file_copy (const char *const source, const char *const destination);
sat_status_t sat_file_move (const char *const source, const char *const destination);
sat_status_t sat_file_get_permissions (const char *const filename, uint32_t *permissions);
sat_status_t sat_file_set_permissions (const char *const filename, uint32_t permissions);
sat_status_t sat_file_check_extension (const char *const filename, const char *const extension);
sat_status_t sat_file_remove (const char *const filename);

#endif/* SAT_FILE_H */
