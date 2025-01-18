#include <stdio.h>
#include <sat.h>
#include <assert.h>
#include <string.h>

typedef struct 
{
    int id;
    char name [100];
    int age;
} person_t;

bool compare (void *element, void *param)
{
    char *name = (char *)param;
    person_t *person = (person_t *)element;

    return strstr (person->name, name);
}

bool is_equal (void *element, void *new_element)
{
    bool status = false;

    person_t *_element = (person_t *)element;
    person_t *_new_element = (person_t *)new_element;

    if (_element->id == _new_element->id)
    {
        status = true;
    }

    return status;
}

int main (int argc, char *argv[])
{

    uint32_t size;

    person_t john = {.id = 1, .name = "John Doe", .age = 35};
    person_t jane = {.id = 2, .name = "Jane Doe", .age = 31};

    person_t recover;

    sat_set_t *set;

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

    status = sat_set_add (set, &jane);
    assert (sat_status_get_result (&status) == false);

    status = sat_set_get_size (set, &size);
    assert (size == 2);

    printf ("Size: %d\n", size);

    status = sat_set_get_object_by (set, 0, &recover);
    assert (sat_status_get_result (&status) == true);

    printf ("Name: %s Age: %d\n", recover.name, recover.age);

    status = sat_set_get_object_by_parameter (set, "Jane", compare, &recover);
    assert (sat_status_get_result (&status) == true);

    status = sat_set_remove_by (set, 0);
    assert (sat_status_get_result (&status) == true);

    status = sat_set_get_size (set, &size);
    assert (size == 1);

    printf ("Size: %d\n", size);

    status = sat_set_destroy (set);
    assert (sat_status_get_result (&status) == true);

    return 0;
}