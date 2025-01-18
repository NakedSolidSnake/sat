#include <sat_file.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

static char *sat_file_get_mode_by (sat_file_mode_t mode);

bool sat_file_open (sat_file_t *object, const char *filename, sat_file_mode_t mode)
{
    bool status = false;

    if (object != NULL && filename != NULL)
    {
        object->handle = fopen (filename, sat_file_get_mode_by (mode));
        if (object->handle != NULL)
        {
            status = true;
        }
    }

    return status;
}

bool sat_file_read (sat_file_t *object, void *buffer, uint32_t size)
{
    bool status = false;
    size_t read_size = 0;

    if (object != NULL && object->handle != NULL && buffer != NULL && size > 0)
    {
        read_size = fread (buffer, 1, size, object->handle);
        status = read_size > 0 && read_size <= size ? true : false;
    }

    return status;
}

bool sat_file_readline (sat_file_t *object, void *buffer, uint32_t size)
{
    bool status = false;

    if (object != NULL && object->handle != NULL && buffer != NULL && size > 0)
    {
        memset (buffer, 0, size);
        status = fgets (buffer, size - 1, object->handle) != NULL ? true : false;
    }

    return status; 
}

bool sat_file_write (sat_file_t *object, const void *buffer, uint32_t size)
{
    bool status = false;
    size_t write_size = 0;

    if (object != NULL && object->handle != NULL && buffer != NULL && size > 0)
    {
        write_size = fwrite (buffer, 1, size, object->handle);

        fflush (object->handle);

        status = write_size == size ? true : false;
    }

    return status;
}

uint32_t sat_file_get_size (sat_file_t *object)
{
    uint32_t size = 0;

    if (object != NULL && object->handle != NULL)
    {
        fseek (object->handle, 0, SEEK_END);
        size = ftell (object->handle);
        rewind (object->handle);     
    }

    return size;
}

bool sat_file_exists (const char *filename)
{
    bool status = false;

    sat_file_t file;

    if (sat_file_open (&file, filename, sat_file_mode_read) == true)
    {
        sat_file_close (&file);

        status = true;
    }

    return status;
}

bool sat_file_read_to_buffer (const char *filename, void **buffer, uint32_t size)
{
    bool status = false;

    sat_file_t file;

    if (buffer != NULL && sat_file_open (&file, filename, sat_file_mode_read) == true)
    {

        uint32_t __size = sat_file_get_size (&file);

        void *__buffer = calloc (1, __size + 1);

        if (__buffer != NULL)
        {
            sat_file_read (&file, __buffer, fmin (size, __size));

            *buffer = __buffer;

            status = true;
        }

        sat_file_close (&file);
    }

    return status;
}

bool sat_file_close (sat_file_t *object)
{
    bool status = false;

    if (object != NULL && object->handle != NULL)
    {
        fclose (object->handle);
        status = true;
    }

    return status;
}

static char *sat_file_get_mode_by (sat_file_mode_t mode)
{
    char *_mode = "w";

    if (mode == sat_file_mode_read)
        _mode = "r";

    else if (mode == sat_file_mode_append)
        _mode = "a";

    return _mode;
}