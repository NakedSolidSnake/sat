#ifndef SAT_DIRECTORY_H
#define SAT_DIRECTORY_H

#include <sat_status.h>
#include <sat_linked_list.h>

sat_status_t sat_directory_exists (const char *const path);
sat_status_t sat_directory_get_files (const char *directory, sat_linked_list_t **file_list);

#endif /* SAT_DIRECTORY_H */
