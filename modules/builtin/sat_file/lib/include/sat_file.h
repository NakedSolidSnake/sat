/**
 * @file sat_file.h
 * @brief File I/O operations and file management utilities
 * @author SAT Library Team
 * @date December 2025
 * 
 * This module provides comprehensive file I/O operations including reading,
 * writing, copying, moving files, and managing file permissions. It offers
 * both low-level operations with explicit handles and high-level convenience
 * functions for common file tasks.
 * 
 * @features
 * - File operations with different access modes (read, write, append)
 * - Line-by-line and bulk data reading
 * - File copying and moving with permission preservation
 * - File existence checking and size retrieval
 * - Permission management (get/set)
 * - File extension validation
 * - Convenient buffer reading functions
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
 */

#ifndef SAT_FILE_H
#define SAT_FILE_H

#include <stdbool.h>
#include <stdint.h>
#include <sat_status.h>

/**
 * @brief File access modes
 * 
 * Defines the mode in which a file is opened, determining available operations.
 */
typedef enum 
{
    sat_file_mode_read,    /**< Read mode - open existing file for reading */
    sat_file_mode_write,   /**< Write mode - create or truncate file for writing */
    sat_file_mode_append   /**< Append mode - open or create file for appending */
} sat_file_mode_t;

/**
 * @brief File handle structure
 * 
 * Opaque structure representing an open file. Use file operations to
 * interact with the file through this handle.
 */
typedef struct 
{
    void *handle;  /**< Internal file handle (FILE*) */
} sat_file_t;

/**
 * @brief Open a file for I/O operations
 * 
 * Opens a file in the specified mode. The file handle is initialized and
 * ready for read/write operations based on the mode.
 * 
 * @param[out] object File object to initialize with the open file handle
 * @param[in] filename Path to the file (relative or absolute)
 * @param[in] mode Access mode (read, write, or append)
 * @return true if file opened successfully, false otherwise
 * 
 * @note In write mode, existing files are truncated
 * @note In append mode, writes occur at the end of the file
 * @note Always call sat_file_close() when done
 * @see sat_file_close()
 */
bool sat_file_open (sat_file_t *object, const char *filename, sat_file_mode_t mode);

/**
 * @brief Read data from an open file
 * 
 * Reads up to the specified number of bytes from the file into the buffer.
 * The file position is advanced by the number of bytes read.
 * 
 * @param[in] object Opened file object
 * @param[out] buffer Buffer to store read data
 * @param[in] size Maximum number of bytes to read
 * @return true if read succeeded, false on error or EOF
 * 
 * @note Reading less than requested bytes is not considered an error
 * @note Buffer must be large enough to hold size bytes
 * @see sat_file_readline(), sat_file_read_to_buffer()
 */
bool sat_file_read (sat_file_t *object, void *buffer, uint32_t size);

/**
 * @brief Read a line from an open file
 * 
 * Reads characters until a newline or EOF is encountered, or the buffer
 * is filled. The newline character is included in the result if present.
 * 
 * @param[in] object Opened file object
 * @param[out] buffer Buffer to store the line
 * @param[in] size Size of the buffer (including space for null terminator)
 * @return true if a line was read, false on error or EOF
 * 
 * @note The buffer is null-terminated
 * @note Use in a loop to read all lines from a file
 * @see sat_file_read()
 */
bool sat_file_readline (sat_file_t *object, void *buffer, uint32_t size);

/**
 * @brief Write data to an open file
 * 
 * Writes the specified number of bytes from the buffer to the file. The
 * data is flushed immediately to ensure it's written to disk.
 * 
 * @param[in] object Opened file object
 * @param[in] buffer Data to write
 * @param[in] size Number of bytes to write
 * @return true if all data was written successfully, false otherwise
 * 
 * @note Returns false if fewer bytes than requested were written
 * @note Data is automatically flushed to disk
 * @see sat_file_open()
 */
bool sat_file_write (sat_file_t *object, const void *buffer, uint32_t size);

/**
 * @brief Get the size of an open file
 * 
 * Determines the size of the file in bytes. The file position is reset
 * to the beginning after determining the size.
 * 
 * @param[in] object Opened file object
 * @return File size in bytes, or 0 if object is invalid
 * 
 * @note The file position is rewound to the beginning
 * @see sat_file_open()
 */
uint32_t sat_file_get_size (sat_file_t *object);

/**
 * @brief Check if a file exists
 * 
 * Tests whether a file exists and is accessible by attempting to open it.
 * 
 * @param[in] filename Path to the file to check
 * @return true if file exists and is accessible, false otherwise
 * 
 * @note This function opens and immediately closes the file
 * @note Returns false if the file exists but is not readable
 */
bool sat_file_exists (const char *filename);

/**
 * @brief Read entire file into a dynamically allocated buffer
 * 
 * Opens a file, reads its contents (or up to a specified size) into a
 * newly allocated buffer, and closes the file. The caller is responsible
 * for freeing the buffer.
 * 
 * @param[in] filename Path to the file to read
 * @param[out] buffer Pointer to receive the allocated buffer
 * @param[in] size Maximum size to read (0 for entire file)
 * @return true if successful, false otherwise
 * 
 * @note Caller must free the allocated buffer using free()
 * @note Buffer is null-terminated
 * @note If size is 0, reads the entire file
 * @see sat_file_read()
 */
bool sat_file_read_to_buffer (const char *filename, void **buffer, uint32_t size);

/**
 * @brief Close an open file
 * 
 * Closes the file handle and releases associated resources. After calling
 * this function, the file object is no longer valid for I/O operations.
 * 
 * @param[in,out] object File object to close
 * @return true if closed successfully, false if object is invalid
 * 
 * @note Always call this function when done with a file
 * @note Flushes any buffered data before closing
 * @see sat_file_open()
 */
bool sat_file_close (sat_file_t *object);

/**
 * @brief Copy a file to a new location
 * 
 * Copies the source file to the destination, preserving file permissions.
 * If the destination exists, it is overwritten.
 * 
 * @param[in] source Path to the source file
 * @param[in] destination Path to the destination file
 * @return Status indicating success or failure
 * 
 * @note File permissions are preserved in the copy
 * @note Destination file is overwritten if it exists
 * @see sat_file_move()
 */
sat_status_t sat_file_copy (const char *const source, const char *const destination);

/**
 * @brief Move a file to a new location
 * 
 * Moves the source file to the destination by copying and then removing
 * the source. Permissions are preserved.
 * 
 * @param[in] source Path to the source file
 * @param[in] destination Path to the destination file
 * @return Status indicating success or failure
 * 
 * @note This is implemented as copy + delete of source
 * @note If copy succeeds but delete fails, both files will exist
 * @see sat_file_copy()
 */
sat_status_t sat_file_move (const char *const source, const char *const destination);

/**
 * @brief Get file permissions
 * 
 * Retrieves the permission bits of a file using stat(). The returned
 * value includes the full mode field (permissions and file type).
 * 
 * @param[in] filename Path to the file
 * @param[out] permissions Pointer to store permission bits
 * @return Status indicating success or failure
 * 
 * @note Returns the full st_mode value from stat()
 * @see sat_file_set_permissions(), chmod(2), stat(2)
 */
sat_status_t sat_file_get_permissions (const char *const filename, uint32_t *permissions);

/**
 * @brief Set file permissions
 * 
 * Sets the permission bits of a file using chmod().
 * 
 * @param[in] filename Path to the file
 * @param[in] permissions Permission bits to set (e.g., 0644)
 * @return Status indicating success or failure
 * 
 * @note Common permissions: 0644 (rw-r--r--), 0755 (rwxr-xr-x)
 * @see sat_file_get_permissions(), chmod(2)
 */
sat_status_t sat_file_set_permissions (const char *const filename, uint32_t permissions);

/**
 * @brief Check if file has a specific extension
 * 
 * Validates that a filename ends with the specified extension.
 * 
 * @param[in] filename Path to the file
 * @param[in] extension Extension to check (without the dot, e.g., "txt")
 * @return Status indicating success (match) or failure (no match)
 * 
 * @note Extension comparison is case-sensitive
 * @note Do not include the dot in the extension parameter
 */
sat_status_t sat_file_check_extension (const char *const filename, const char *const extension);

/**
 * @brief Remove a file
 * 
 * Deletes the specified file from the filesystem.
 * 
 * @param[in] filename Path to the file to remove
 * @return Status indicating success or failure
 * 
 * @note File is permanently deleted
 * @note Returns error if file doesn't exist or cannot be deleted
 * @see remove(3)
 */
sat_status_t sat_file_remove (const char *const filename);

#endif/* SAT_FILE_H */
