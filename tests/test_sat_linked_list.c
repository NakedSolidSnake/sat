#include <sat.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

typedef struct 
{
    char name [100];
    int age;
} person_t;

static void print (void *element)
{
    person_t *person = (person_t *)element;
    printf ("Name: %s, Age: %d\n", person->name, person->age);
}

static bool compare (void *element, void *data)
{
    person_t *person = (person_t *)element;
    char *name = (char *)data;

    return strstr (person->name, name) != NULL;
}

int main (int argc, char *argv[])
{
    uint32_t size = 0;

    person_t john = {.name = "John Doe", .age = 35};
    person_t jane = {.name = "Jane Doe", .age = 31};

    person_t recover;

    sat_linked_list_t *linked;

    sat_status_t status = sat_linked_list_create (&linked, sizeof (person_t));
    assert (sat_status_get_result (&status) == true);

    status = sat_linked_list_insert (linked, &john);
    assert (sat_status_get_result (&status) == true);

    status = sat_linked_list_insert (linked, &jane);
    assert (sat_status_get_result (&status) == true);

    status = sat_linked_list_get_size (linked, &size);
    assert (sat_status_get_result (&status) == true);

    assert (size == 2);

    sat_linked_list_debug (linked, print);

    status = sat_linked_list_get (linked, compare, "Jane", &recover);
    assert (sat_status_get_result (&status) == true);

    assert (strcmp (recover.name, jane.name) == 0);

    status = sat_linked_list_remove (linked, compare, "John");
    assert (sat_status_get_result (&status) == true);

    status = sat_linked_list_get_size (linked, &size);
    assert (sat_status_get_result (&status) == true);

    assert (size == 1);

    sat_linked_list_debug (linked, print);

    status = sat_linked_list_destroy (linked);
    assert (sat_status_get_result (&status) == true);

}