#include <sat.h>
#include <unistd.h>
#include <assert.h>
#include <stdio.h>

typedef struct 
{
    char *name;
} person_t;

void handler (void *object);

int main (int argc, char *argv[])
{
    sat_status_t status;

    person_t persons [] = 
    {
        {.name = "John Doe"},
        {.name = "Jane Doe"}
    };

    sat_worker_t worker;

    sat_worker_args_t args = 
    {
        .handler = handler,
        .object_size = sizeof (person_t),
        .pool_amount = 10
    };

    status = sat_worker_init (&worker);
    assert (sat_status_get_result (&status) == true);

    status = sat_worker_open (&worker, &args);
    assert (sat_status_get_result (&status) == true);

    for (int j = 0; j < 100; j++)
    {
        for (int i = 0; i < 2; i++)
        {
            status = sat_worker_feed (&worker, &persons [i]);
            assert (sat_status_get_result (&status) == true);
        }
    }

    sleep (1);

    status = sat_worker_close (&worker);
    assert (sat_status_get_result (&status) == true);

    return 0;
}

void handler (void *object)
{
    person_t *person = (person_t *)object;

    printf ("%s\n", person->name);
}