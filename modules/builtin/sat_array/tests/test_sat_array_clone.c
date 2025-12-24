#include <sat.h>
#include <assert.h>
#include <string.h>

typedef struct
{
    char name [50];
    int age;
} person_t;

int main (void)
{
    person_t persons [3] =
    {
        { "Alice", 30 },
        { "Bob", 25 },
        { "Charlie", 35 }
    };

    sat_array_t *array;
    sat_array_t *clone;

    sat_status_t status = sat_array_create (&array, &(sat_array_args_t)
                                                    {
                                                        .size = 3,
                                                        .object_size = sizeof (person_t),
                                                        .mode = sat_array_mode_static
                                                    });
    assert (sat_status_get_result (&status) == true);

    for (uint32_t i = 0; i < 3; i++)
    {
        status = sat_array_add (array, &persons [i]);
        assert (sat_status_get_result (&status) == true);
    }

    status = sat_array_clone (array, &clone);
    assert (sat_status_get_result (&status) == true);

    sat_iterator_t iterator;
    status = sat_iterator_open (&iterator, (sat_iterator_base_t *)clone);
    assert (sat_status_get_result (&status) == true);

    uint32_t index = 0;
    person_t *current_person = sat_iterator_next (&iterator);
    while (current_person != NULL)
    {
        assert (current_person->age == persons [index].age);
        assert (strcmp (current_person->name, persons [index].name) == 0);
        index++;
        current_person = sat_iterator_next (&iterator);
    }

    sat_array_destroy (array);
    sat_array_destroy (clone);

    return 0;                                                    
}