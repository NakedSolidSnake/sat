#include <sat_file.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/stat.h>

static char *sat_file_get_mode_by (sat_file_mode_t mode);

sat_status_t sat_file_open (sat_file_t *const object, const char *const filename, sat_file_mode_t mode)
{
    sat_status_return_on_null (object, "object pointer is null");
    sat_status_return_on_null (filename, "filename pointer is null");
    sat_status_return_on_equals (strlen (filename), 0, "filename is empty");

    object->handle = fopen (filename, sat_file_get_mode_by (mode));
    sat_status_return_on_null (object->handle, "failed to open file");

    sat_status_return_on_success ();
}

sat_status_t sat_file_read (const sat_file_t *const object, void *const buffer, uint32_t size)
{
    size_t read_size = 0;
    
    sat_status_return_on_null (object, "object pointer is null");
    sat_status_return_on_null (object->handle, "file handle is null");
    sat_status_return_on_null (buffer, "buffer pointer is null");
    sat_status_return_on_equals (size, 0, "size is zero");

    read_size = fread (buffer, 1, size, object->handle);

    sat_status_return_on_equals (read_size, 0, "failed to read from file");
    sat_status_return_on_greater_than (read_size, size, "read size exceeds buffer size");

    sat_status_return_on_success ();
}

sat_status_t sat_file_readline (const sat_file_t *const object, void *const buffer, uint32_t size)
{
    sat_status_return_on_null (object, "object pointer is null");
    sat_status_return_on_null (object->handle, "file handle is null");
    sat_status_return_on_null (buffer, "buffer pointer is null");
    sat_status_return_on_equals (size, 0, "size is zero");

    memset (buffer, 0, size);

    sat_status_return_on_null (fgets (buffer, size - 1, object->handle), "failed to read line from file");

    sat_status_return_on_success ();
}

sat_status_t sat_file_write (const sat_file_t *const object, const void *buffer, uint32_t size)
{
    size_t write_size = 0;

    sat_status_return_on_null (object, "object pointer is null");
    sat_status_return_on_null (object->handle, "file handle is null");
    sat_status_return_on_null (buffer, "buffer pointer is null");
    sat_status_return_on_equals (size, 0, "size is zero");

    write_size = fwrite (buffer, 1, size, object->handle);
    
    sat_status_return_on_not_equals (write_size, size, "failed to write all data to file");

    fflush (object->handle);

    sat_status_return_on_success ();
}

sat_status_t sat_file_get_size (const sat_file_t *const object, uint32_t *const size)
{
    sat_status_return_on_null (object, "object pointer is null");
    sat_status_return_on_null (object->handle, "file handle is null");
    sat_status_return_on_null (size, "size pointer is null");

    fseek (object->handle, 0, SEEK_END);
    *size = ftell (object->handle);
    rewind (object->handle);

    sat_status_return_on_success ();
}

sat_status_t sat_file_exists (const char *filename)
{
    sat_file_t file;

    sat_status_return_on_error (sat_file_open (&file, filename, sat_file_mode_read));

    sat_status_return_on_error (sat_file_close (&file));

    sat_status_return_on_success ();
}

sat_status_t sat_file_read_to_buffer (const char *const filename, void **const buffer, uint32_t size)
{
    sat_file_t file;
    uint32_t file_size;

    sat_status_return_on_null (buffer, "buffer pointer is null");
    sat_status_return_on_equals (size, 0, "size is zero");
    sat_status_return_on_error (sat_file_open (&file, filename, sat_file_mode_read));

    sat_status_t status = sat_file_get_size (&file, &file_size);
    if (sat_status_get_result (&status) == false)
    {
        sat_file_close (&file);
        return status;
    }

    void *__buffer = calloc (1, file_size + 1);

    if (__buffer == NULL)
    {
        sat_file_close (&file);
        sat_status_return_on_failure ("failed to allocate buffer for file contents");
    }
    
    status = sat_file_read (&file, __buffer, fmin (size, file_size));
    if (sat_status_get_result (&status) == false)
    {
        free (__buffer);
        sat_file_close (&file);
        return status;
    }

    *buffer = __buffer;

    sat_file_close (&file);
    
    sat_status_return_on_success ();
}

sat_status_t sat_file_close (sat_file_t *const object)
{
    sat_status_return_on_null (object, "object pointer is null");
    sat_status_return_on_null (object->handle, "file handle is null");

    fclose (object->handle);
    object->handle = NULL;

    sat_status_return_on_success ();
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

