/**
 * @file sat_directory.h
 * @brief Directory operations and file listing utilities
 * @author Cristiano Silva de Souza
 * @date 2025
 * 
 * This module provides utilities for working with directories, including
 * checking directory existence and retrieving lists of files within directories.
 * File lists are returned as linked lists for easy iteration and manipulation.
 */

#ifndef SAT_DIRECTORY_H
#define SAT_DIRECTORY_H

#include <sat_status.h>
#include <sat_linked_list.h>

/**
 * @brief Checks if a directory exists
 * 
 * Verifies whether the specified path exists and is a directory.
 * 
 * @param[in] path Path to the directory to check
 * @return sat_status_t indicating success if directory exists, failure otherwise
 * 
 * @note This function does not check if the directory is readable or accessible
 * @note The path can be absolute or relative
 */
sat_status_t sat_directory_exists (const char *const path);

/**
 * @brief Retrieves a list of files in a directory
 * 
 * Scans the specified directory and creates a linked list containing the
 * names of all files found. The list can be iterated using sat_iterator
 * or sat_linked_list functions.
 * 
 * @param[in] directory Path to the directory to scan
 * @param[out] file_list Pointer to receive the created linked list of filenames
 * @return sat_status_t indicating success or failure of the operation
 * 
 * @note The caller is responsible for destroying the file_list using sat_linked_list_destroy()
 * @note Each element in the list is a string (char*) containing a filename
 * @note Only files are included; subdirectories are typically excluded
 * @note The directory path can be absolute or relative
 * @warning The file_list must be destroyed after use to prevent memory leaks
 * @see sat_linked_list_destroy()
 * @see sat_iterator
 */
sat_status_t sat_directory_get_files (const char *directory, sat_linked_list_t **file_list);

#endif /* SAT_DIRECTORY_H */
