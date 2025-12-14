#include <sat.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

typedef struct 
{
    char name [512];
    int age;
} person_t;

int main (int argc, char *argv[])
{
    sat_cache_t cache;

    person_t person = {.name = "John Doe", .age = 36};
    person_t recover;

    sat_status_t status = sat_cache_init (&cache);
    assert (sat_status_get_result (&status) == true);

    status = sat_cache_open (&cache, &(sat_cache_args_t){.buffer_size = 1024});
    assert (sat_status_get_result (&status) == true);

    status = sat_cache_is_cached (&cache);
    assert (sat_status_get_result (&status) == false);

    status = sat_cache_store (&cache, &person, sizeof (person_t));
    assert (sat_status_get_result (&status) == true);

    status = sat_cache_is_cached (&cache);
    assert (sat_status_get_result (&status) == true);

    status = sat_cache_restore (&cache, &recover, sizeof (person_t));
    assert (sat_status_get_result (&status) == true);

    assert (strcmp (recover.name, "John Doe") == 0);
    assert (recover.age == 36);

    status = sat_cache_clear (&cache);
    assert (sat_status_get_result (&status) == true);

    status = sat_cache_is_cached (&cache);
    assert (sat_status_get_result (&status) == false);

    status = sat_cache_close (&cache);
    assert (sat_status_get_result (&status) == true);

    return 0;
}