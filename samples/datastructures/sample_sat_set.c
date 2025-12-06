#include <stdio.h>
#include <sat_set.h>
#include <assert.h>
#include <string.h>

typedef struct 
{
    int id;
    char name [100];
    int age;
} person_t;

bool is_equal (const void *const element, const void *const new_element)
{
    bool status = false;

    const person_t *_element = (const person_t *)element;
    const person_t *_new_element = (const person_t *)new_element;

    if (_element->id == _new_element->id)
    {
        status = true;
    }

    return status;
}

bool compare_by_name (const void *const element, const void *const param)
{
    const char *name = (const char *)param;
    const person_t *person = (const person_t *)element;

    return strcmp (person->name, name) == 0;
}

void test_remove_by_parameter (void);

int main (int argc, char *argv[])
{
    test_remove_by_parameter ();

    return 0;
}

void test_remove_by_parameter (void)
{
    uint32_t size;

    person_t john = {.id = 1, .name = "John Doe",   .age = 35};
    person_t jane = {.id = 2, .name = "Jane Doe",   .age = 31};
    person_t jack = {.id = 3, .name = "Jack Smith", .age = 28};

    person_t recover;

    sat_set_t *set;

    sat_status_t status = sat_set_create (&set, &(sat_set_args_t)
                                                    {
                                                        .size = 2,
                                                        .object_size = sizeof (person_t),
                                                        .is_equal = is_equal,
                                                        .mode = sat_set_mode_dynamic,
                                                    });
    assert (sat_status_get_result (&status) == true);

    status = sat_set_add (set, &john);
    assert (sat_status_get_result (&status) == true);

    status = sat_set_add (set, &jane);
    assert (sat_status_get_result (&status) == true);

    status = sat_set_add (set, &jack);
    assert (sat_status_get_result (&status) == true);

    status = sat_set_remove_by_parameter (set, "Jane Doe", compare_by_name, &recover);
    assert (sat_status_get_result (&status) == true);

    printf ("Removed Name: %s Age: %d\n", recover.name, recover.age);

    status = sat_set_get_size (set, &size);
    assert (size == 2);

    printf ("Size after removal: %d\n", size);

    status = sat_set_destroy (set);
    assert (sat_status_get_result (&status) == true);
}