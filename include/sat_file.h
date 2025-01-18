#ifndef SAT_FILE_H
#define SAT_FILE_H

#include <stdbool.h>
#include <stdint.h>

typedef enum 
{
    sat_file_mode_read,
    sat_file_mode_write,
    sat_file_mode_append
} sat_file_mode_t;

typedef struct 
{
    void *handle;
} sat_file_t;

bool sat_file_open (sat_file_t *object, const char *filename, sat_file_mode_t mode);
bool sat_file_read (sat_file_t *object, void *buffer, uint32_t size);
bool sat_file_readline (sat_file_t *object, void *buffer, uint32_t size);
bool sat_file_write (sat_file_t *object, const void *buffer, uint32_t size);
uint32_t sat_file_get_size (sat_file_t *object);
bool sat_file_exists (const char *filename);
bool sat_file_read_to_buffer (const char *filename, void **buffer, uint32_t size);
bool sat_file_close (sat_file_t *object);

#endif/* SAT_FILE_H */
