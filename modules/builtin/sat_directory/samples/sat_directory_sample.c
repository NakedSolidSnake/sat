#include <sat.h>

int main (int argc, char *argv [])
{
    sat_linked_list_t *file_list;
    sat_status_t status;
    uint32_t size;
    sat_iterator_t iterator;
    sat_iterator_base_t *base;

    if (argc < 2)
    {
        printf ("Usage: %s <directory_path>\n", argv[0]);
        return -1;
    }

    status = sat_directory_get_files (argv[1], &file_list);
    if (sat_status_get_result (&status) == false)
    {
        printf ("Failed to get files in directory: %s\n", sat_status_get_motive (&status));
        return -1;
    }

    status = sat_linked_list_get_size (file_list, &size);
    if (sat_status_get_result (&status) == false)
    {
        printf ("Failed to get file list size: %s\n", sat_status_get_motive (&status));
        return -1;
    }
    
    printf ("Files found: %u\n", size);

    base = (sat_iterator_base_t *)file_list;
    status = sat_iterator_open (&iterator, base);
    if (sat_status_get_result (&status) == false)
    {
        printf ("Failed to open iterator: %s\n", sat_status_get_motive (&status));
        return -1;
    }

    // Iterate through files and print names
    char *current_file;
    printf ("--- File List ---\n");
    while ((current_file = (char *)sat_iterator_next (&iterator)) != NULL)
    {
        printf ("%s\n", current_file);
    }
    printf ("-----------------\n");

    // Cleanup
    status = sat_linked_list_destroy (file_list);
    if (sat_status_get_result (&status) == false)
    {
        printf ("Failed to destroy file list: %s\n", sat_status_get_motive (&status));
        return -1;
    }

    return 0;
}