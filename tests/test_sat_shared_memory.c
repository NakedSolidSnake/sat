#include <sat.h>
#include <assert.h>
#include <string.h>

#define NAME_SIZE       80

typedef struct 
{
    char name [NAME_SIZE + 1];
    int age;
} person_t;

int main (int argc, char *argv [])
{
    sat_shared_memory_t shm;
    person_t *person_write;
    person_t *person_read;

    sat_status_t status =  sat_shared_memory_open (&shm, &(sat_shared_memory_args_t)
                                                         {
                                                            .size = sizeof (person_t),
                                                            .flags = sat_shared_memory_flags_all_read_write,
                                                            .key = 64
                                                         });
    assert (sat_status_get_result (&status) == true);

    status = sat_shared_memory_get (&shm, (void **)&person_write);
    assert (sat_status_get_result (&status) == true);

    person_write->age = 10;
    strncpy (person_write->name, "John Doe", NAME_SIZE);

    status = sat_shared_memory_get (&shm, (void **)&person_read);
    assert (sat_status_get_result (&status) == true);

    assert (person_read->age == 10);
    assert (strcmp (person_read->name, "John Doe") == 0);

    status = sat_shared_memory_detach (&shm);
    assert (sat_status_get_result (&status) == true);

    status = sat_shared_memory_destroy (&shm);
    assert (sat_status_get_result (&status) == true);

    return 0;
}