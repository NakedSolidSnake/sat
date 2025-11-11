#include <sat_directory.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <limits.h>
#include <string.h>

sat_status_t sat_directory_exists (const char *const path)
{
    struct stat info;
    sat_status_t status;

    do 
    {
        if (stat (path, &info) != 0)
        {
            sat_status_failure (&status, "Unable to access path.");
            break;
        }

        if (S_ISDIR (info.st_mode) == 0)
        {
            sat_status_failure (&status, "Path is not a directory.");
            break;
        }

        sat_status_success (&status);

    } while (false);

    return status;  
}

sat_status_t sat_directory_get_files (const char *directory, sat_linked_list_t **file_list)
{
    sat_status_t status;

    DIR *dir = NULL;
    struct dirent *entry = NULL;
    char filename [NAME_MAX + 1] = {0};

    do
    {
        if (directory == NULL || file_list == NULL)
        {
            status = sat_status_failure (&status, "Invalid arguments");
            break;
        }

        status = sat_directory_exists (directory);
        if (sat_status_get_result (&status) == false)
        {
            break;
        }

        status = sat_linked_list_create (file_list, NAME_MAX + 1);
        if (sat_status_get_result (&status) == false)
        {
            break;
        }

        dir = opendir (directory);
        if (dir == NULL)
        {
            sat_linked_list_destroy (*file_list);
            status = sat_status_failure (&status, "Failed to open directory");
            break;
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
                    break;
                }
            }
        }

        closedir (dir);

    } while (false);

    return status;
}