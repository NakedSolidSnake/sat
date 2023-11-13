#include <sat.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct 
{
    char name [100];
} person_t;

void on_read (char *content, void *data)
{
    person_t *person = (person_t *)data;
    strncpy (person->name, content, 99);
}

int main (int argc, char *argv[])
{
    sat_redis_t redis;
    uint8_t retries = 0;

    person_t person;

    char *docker_path = argv[1];

    chdir (docker_path);

    system ("docker compose up -d");

    sleep (2);

    memset (&person, 0, sizeof (person_t));

    sat_status_t status = sat_redis_init (&redis);
    assert (sat_status_get_result (&status) == true);

    status = sat_redis_open (&redis, &(sat_redis_args_t){.host = "localhost", .port = 6379});    
    assert (sat_status_get_result (&status) == true);

    do
    {
        sat_redis_save (&redis, "name", "John Doe");
        status = sat_redis_is_key_exists (&redis, "name");

        retries ++;
        sleep (1);
    } while (sat_status_get_result (&status) == false && retries < 5);
    
    assert (sat_status_get_result (&status) == true);

    status = sat_redis_get (&redis, "name", on_read, &person);
    assert (sat_status_get_result (&status) == true);

    assert (strcmp (person.name, "John Doe") == 0);

    printf ("Name: %s\n", person.name);

    status = sat_redis_close (&redis);
    assert (sat_status_get_result (&status) == true);

    system ("docker compose down");
    return 0;
}