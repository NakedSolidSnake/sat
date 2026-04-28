#include <sat_directory.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <limits.h>
#include <string.h>

sat_status_t sat_directory_exists (const char *const path)
{
    struct stat info;
    
    sat_status_return_on_null (path, "path pointer is NULL");
    sat_status_return_on_equals (strlen (path), 0, "path is empty string");
    sat_status_return_on_not_equals (stat (path, &info), 0, "unable to access path");
    sat_status_return_on_equals (S_ISDIR (info.st_mode), 0, "path is not a directory");

    sat_status_return_on_success ();
}

sat_status_t sat_directory_get_files (const char *directory, sat_linked_list_t **const file_list)
{
    DIR *dir = NULL;
    struct dirent *entry = NULL;
    char filename [NAME_MAX + 1] = {0};
    sat_status_t status;

    sat_status_return_on_null (directory, "directory pointer is NULL");
    sat_status_return_on_null (file_list, "file list pointer is NULL");
    sat_status_return_on_error (sat_directory_exists (directory));
    sat_status_return_on_error (sat_linked_list_create (file_list, NAME_MAX + 1));

    dir = opendir (directory);
    if (dir == NULL)
    {
        sat_linked_list_destroy (*file_list);
        sat_status_return_on_failure ("Failed to open directory");
    }

    while ((entry = readdir (dir)) != NULL)
    {
        if (entry->d_type == DT_REG)
        {
            strncpy (filename, entry->d_name, NAME_MAX);
            filename [NAME_MAX] = '\0';

            status = sat_linked_list_insert (*file_list, filename);
            if (sat_status_get_result (&status) == false)
            {
                sat_linked_list_destroy (*file_list);
                *file_list = NULL;
                closedir (dir);
                sat_status_return_on_failure ("Failed to insert filename into list");
            }
        }
    }

    closedir (dir);

    sat_status_return_on_success ();
}
