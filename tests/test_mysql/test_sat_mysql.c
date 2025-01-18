#include <sat.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>

typedef struct 
{
    char name [100];
} person_t;

void on_result_set (char **data, int columns, void *user);

int main (int argc, char *argv[])
{
    person_t person;
    sat_mysql_t mysql;
    sat_status_t status;
    uint8_t retries = 0;

    char *docker_path = argv[1];

    chdir (docker_path);

    system ("docker compose up -d");

    sat_mysql_args_t args = 
    {
        .database = "test_db",
        .hostname = "127.0.0.1",
        .user = "root",
        .password = "root",
        .port = 9998
    };

    status = sat_mysql_init (&mysql);
    assert (sat_status_get_result (&status) == true);

    do 
    {
        status = sat_mysql_open (&mysql, &args);        
        sleep (1);
        retries ++;

    } while (sat_status_get_result (&status) == false && retries < 20);

    assert (sat_status_get_result (&status) == true);

    status = sat_mysql_execute (&mysql, "DELETE FROM persons_tb;");
    assert (sat_status_get_result (&status) == true);

    status = sat_mysql_execute (&mysql, "INSERT INTO persons_tb (name) values ('John Doe');");
    assert (sat_status_get_result (&status) == true);

    status = sat_mysql_execute (&mysql, "SELECT * FROM persons_tb;");
    assert (sat_status_get_result (&status) == true);

    status = sat_mysql_result_set (&mysql, on_result_set, &person);
    assert (sat_status_get_result (&status) == true);

    assert (strcmp (person.name, "John Doe") == 0);

    status = sat_mysql_execute (&mysql, "UPDATE persons_tb SET name = 'Jane Doe' WHERE name = 'John Doe';");
    assert (sat_status_get_result (&status) == true);

    status = sat_mysql_execute (&mysql, "SELECT * FROM persons_tb;");
    assert (sat_status_get_result (&status) == true);

    status = sat_mysql_result_set (&mysql, on_result_set, &person);
    assert (sat_status_get_result (&status) == true);

    assert (strcmp (person.name, "Jane Doe") == 0);

    status = sat_mysql_close (&mysql);
    assert (sat_status_get_result (&status) == true);    

    system ("docker compose down");

}

void on_result_set (char **data, int columns, void *user)
{
    person_t *person = (person_t *)user;
    strncpy (person->name, data [1], 100);
}