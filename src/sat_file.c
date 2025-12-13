#include <sat_file.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/stat.h>

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

sat_status_t sat_file_copy (const char *const source, const char *const destination)
{
    sat_status_t status;
    char buffer [4096];
    size_t bytes;
    uint32_t permissions;

    do
    {
        FILE *__source = fopen (source, "rb");
        if (__source == NULL)
        {
            sat_status_failure (&status, "sat file copy error: unable to open source file");

            break;
        }

        FILE * __destination = fopen (destination, "wb");
        if (__destination == NULL)
        {
            fclose (__source);
            
            sat_status_failure (&status, "sat file copy error: unable to open destination file");

            break;
        }


        while ((bytes = fread (buffer, 1, sizeof (buffer), __source)) > 0)
        {
            fwrite (buffer, 1, bytes, __destination);
        }

        fclose (__source);
        fclose (__destination);

        status = sat_file_get_permissions (source, &permissions);
        sat_status_break_on_error (status);

        status = sat_file_set_permissions (destination, permissions);
        sat_status_break_on_error (status);

        sat_status_success (&status);

    } while (false);

    return status;
}

sat_status_t sat_file_move (const char *const source, const char *const destination)
{
    sat_status_t status = sat_file_copy (source, destination);
    if (sat_status_get_result (&status) == true)
    {
        status = sat_file_remove (source);
    }
    else
    {
        status = sat_file_remove (destination);
    }

    return status;
}

sat_status_t sat_file_get_permissions (const char *const filename, uint32_t *permissions)
{
    sat_status_t status = sat_status_failure (&status, "sat file get permissions error: unable to get file status");

    struct stat file_stat;

    if (stat (filename, &file_stat) == 0)
    {
        *permissions = file_stat.st_mode;

        sat_status_success (&status);
    }

    return status;
}

sat_status_t sat_file_set_permissions (const char *const filename, uint32_t permissions)
{
    sat_status_t status = sat_status_failure (&status, "sat file set permissions error: unable to set file permissions");

    if (chmod (filename, permissions) == 0)
    {
        sat_status_success (&status);
    }

    return status;
}

sat_status_t sat_file_check_extension (const char *const filename, const char *const extension)
{
    sat_status_t status = sat_status_failure (&status, "sat file check extension error: file extension does not match");;

    const char *file_ext = strrchr (filename, '.');

    if (file_ext != NULL && strcmp (file_ext + 1, extension) == 0)
    {
        sat_status_success (&status);
    }

    return status;
}

sat_status_t sat_file_remove (const char *const filename)
{
    sat_status_t status = sat_status_failure (&status, "sat file remove error: unable to remove file");

    if (remove (filename) == 0)
    {
        sat_status_success (&status);
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

