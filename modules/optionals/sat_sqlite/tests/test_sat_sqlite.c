#include <sat.h>
#include <assert.h>
#include <string.h>

typedef struct 
{
    char name [100];
} person_t;

int on_result_set (void *user, int columns, char **data, char **names);

char *query = "CREATE TABLE IF NOT EXISTS persons_tb (" \
              "ID INTEGER PRIMARY KEY AUTOINCREMENT," \
              "NAME TEXT NOT NULL);";

int main (int argc, char *argv[])
{
    person_t person;
    sat_sqlite_t sqlite;
    sat_status_t status;

    sat_sqlite_args_t args = 
    {
        .database = "test_db"
    };

    status = sat_sqlite_init (&sqlite);
    assert (sat_status_get_result (&status) == true);

    status = sat_sqlite_open (&sqlite, &args);
    assert (sat_status_get_result (&status) == true);

    status = sat_sqlite_execute (&sqlite, query, NULL, NULL);
    assert (sat_status_get_result (&status) == true);

    status = sat_sqlite_execute (&sqlite, "DELETE FROM persons_tb;", NULL, NULL);
    assert (sat_status_get_result (&status) == true);

    status = sat_sqlite_execute (&sqlite, "INSERT INTO persons_tb (name) values ('John Doe');", NULL, NULL);
    assert (sat_status_get_result (&status) == true);

    status = sat_sqlite_execute (&sqlite, "SELECT * FROM persons_tb;", on_result_set, &person);
    assert (sat_status_get_result (&status) == true);

    assert (strcmp (person.name, "John Doe") == 0);

    status = sat_sqlite_execute (&sqlite, "UPDATE persons_tb SET name = 'Jane Doe' WHERE name = 'John Doe';", NULL, NULL);
    assert (sat_status_get_result (&status) == true);

    status = sat_sqlite_execute (&sqlite, "SELECT * FROM persons_tb;", on_result_set, &person);
    assert (sat_status_get_result (&status) == true);

    assert (strcmp (person.name, "Jane Doe") == 0);

    status = sat_sqlite_close (&sqlite);
    assert (sat_status_get_result (&status) == true);    

}

int on_result_set (void *user, int columns, char **data, char **names)
{
    person_t *person = (person_t *)user;
    strncpy (person->name, data [1], 100);
    return 0;
}