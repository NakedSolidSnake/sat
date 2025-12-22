#include <sat.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct 
{
    char name [100];
} person_t;

void on_result_set (char **data, int columns, void *user);

int main (int argc, char *argv[])
{
    person_t person;
    sat_postgres_t postgres;
    sat_status_t status;
    uint8_t retries = 0;

    char *docker_path = argv[1];

    chdir (docker_path);

    system ("docker compose up -d");

    sat_postgres_args_t args = 
    {
        .database = "test_db",
        .hostname = "localhost",
        .user = "root",
        .password = "root",
        .port = 5432
    };

    status = sat_postgres_init (&postgres);
    assert (sat_status_get_result (&status) == true);

    do 
    {
        status = sat_postgres_open (&postgres, &args);
        sleep (1);
        retries ++;

    } while (sat_status_get_result (&status) == false && retries < 20);
    
    assert (sat_status_get_result (&status) == true);

    status = sat_postgres_execute (&postgres, "DELETE FROM persons_tb;");
    assert (sat_status_get_result (&status) == true);

    status = sat_postgres_execute (&postgres, "INSERT INTO persons_tb (name) values ('John Doe');");
    assert (sat_status_get_result (&status) == true);

    status = sat_postgres_execute (&postgres, "SELECT * FROM persons_tb;");
    assert (sat_status_get_result (&status) == true);

    status = sat_postgres_result_set (&postgres, on_result_set, &person);
    assert (sat_status_get_result (&status) == true);

    assert (strcmp (person.name, "John Doe") == 0);

    status = sat_postgres_execute (&postgres, "UPDATE persons_tb SET name = 'Jane Doe' WHERE name = 'John Doe';");
    assert (sat_status_get_result (&status) == true);

    status = sat_postgres_execute (&postgres, "SELECT * FROM persons_tb;");
    assert (sat_status_get_result (&status) == true);

    status = sat_postgres_result_set (&postgres, on_result_set, &person);
    assert (sat_status_get_result (&status) == true);

    assert (strcmp (person.name, "Jane Doe") == 0);

    status = sat_postgres_close (&postgres);
    assert (sat_status_get_result (&status) == true);    

    system ("docker compose down");

}

void on_result_set (char **data, int columns, void *user)
{
    person_t *person = (person_t *)user;
    strncpy (person->name, data [1], 100);
}