#include <stdio.h>
#include <sat.h>
#include <assert.h>
#include <string.h>

typedef struct 
{
    char name [100];
    int age;
} person_t;

static void sat_array_iterator_test (void);
static void sat_set_iterator_test (void);
static bool is_equal (void *element, void *new_element);

int main (int argc, char *argv[])
{
    sat_array_iterator_test ();
    sat_set_iterator_test ();

    return 0;
}

static void sat_array_iterator_test (void)
{
    person_t john = {.name = "John Doe", .age = 35};
    person_t jane = {.name = "Jane Doe", .age = 31};
    person_t some = {.name = "Some One", .age = 33};


    sat_array_t *array;
    sat_iterator_t iterator;

    sat_status_t status = sat_array_create (&array, &(sat_array_args_t)
                                                    {
                                                        .size = 3,
                                                        .object_size = sizeof (person_t),
                                                        .mode = sat_array_mode_static,
                                                    });
    assert (sat_status_get_result (&status) == true);

    status = sat_array_add (array, &john);
    assert (sat_status_get_result (&status) == true);

    status = sat_array_add (array, &jane);
    assert (sat_status_get_result (&status) == true);

    status = sat_array_add (array, &some);
    assert (sat_status_get_result (&status) == true);

    status = sat_iterator_open (&iterator, (sat_iterator_base_t *)array);
    assert (sat_status_get_result (&status) == true);

    person_t *person = (person_t *) sat_iterator_next (&iterator);
    while (person != NULL)
    {
        printf ("Name: %s Age: %d\n", person->name, person->age);
        person = (person_t *) sat_iterator_next (&iterator);
    }

    status = sat_array_destroy (array);
    assert (sat_status_get_result (&status) == true);
}

static void sat_set_iterator_test (void)
{
    person_t john = {.name = "John Doe", .age = 35};
    person_t jane = {.name = "Jane Doe", .age = 31};
    person_t some = {.name = "Some One", .age = 33};


    sat_set_t *set;
    sat_iterator_t iterator;

    sat_status_t status = sat_set_create (&set, &(sat_set_args_t)
                                                {
                                                    .size = 3,
                                                    .object_size = sizeof (person_t),
                                                    .is_equal = is_equal,
                                                    .mode = sat_set_mode_static
                                                });
    assert (sat_status_get_result (&status) == true);

    status = sat_set_add (set, &john);
    assert (sat_status_get_result (&status) == true);

    status = sat_set_add (set, &jane);
    assert (sat_status_get_result (&status) == true);

    status = sat_set_add (set, &some);
    assert (sat_status_get_result (&status) == true);

    status = sat_iterator_open (&iterator, (sat_iterator_base_t *)set);
    assert (sat_status_get_result (&status) == true);

    person_t *person = (person_t *) sat_iterator_next (&iterator);
    while (person != NULL)
    {
        printf ("Name: %s Age: %d\n", person->name, person->age);
        person = (person_t *) sat_iterator_next (&iterator);
    }

    status = sat_set_destroy (set);
    assert (sat_status_get_result (&status) == true);
}


static bool is_equal (void *element, void *new_element)
{
    return false;
}