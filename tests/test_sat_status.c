#include <stdio.h>
#include <sat.h>
#include <string.h>
#include <assert.h>

typedef struct 
{
    char name [100];
} person_t;

sat_status_t person_create (person_t *person, const char *name);

int main (int argc, char *argv[])
{
    person_t person;

    sat_status_t status = person_create (&person, "Cristiano Silva");
    assert (sat_status_get_result (&status) == true);
    puts ("✓ Person created successfully");

    status = person_create (&person, "");
    assert (sat_status_get_result (&status) == false);
    assert (strcmp (sat_status_get_motive (&status), "person name parameter error") == 0);
    puts ("✓ Person creation failed due to name parameter error");

    return 0;
}

sat_status_t person_create (person_t *person, const char *name)
{
    sat_status_t status = sat_status_failure (&status, "person create error");

    if (person != NULL)
    {
        status = sat_status_failure (&status, "person name parameter error");
        if (name != NULL && strlen (name) > 0)
        {
            sat_status_success (&status);
        }
    }

    return status;
}