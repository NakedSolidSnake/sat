#include <stdio.h>
#include <sat_array.h>
#include <assert.h>
#include <string.h>

typedef struct 
{
    char name [100];
    int age;
} person_t;

bool compare (void *element, void *param)
{
    char *name = (char *)param;
    person_t *person = (person_t *)element;

    return strstr (person->name, name);
}

int main (int argc, char *argv[])
{

    uint32_t size;

    person_t john = {.name = "John Doe", .age = 35};
    person_t jane = {.name = "Jane Doe", .age = 31};

    person_t recover;

    sat_array_t *array;

    sat_status_t status = sat_array_create (&array, 2, sizeof (person_t));
    assert (sat_status_get_result (&status) == true);

    status = sat_array_add (array, &john);
    assert (sat_status_get_result (&status) == true);

    status = sat_array_add (array, &jane);
    assert (sat_status_get_result (&status) == true);

    status = sat_array_get_size (array, &size);
    assert (size == 2);

    printf ("Size: %d\n", size);

    status = sat_array_get_object_by (array, 0, &recover);
    assert (sat_status_get_result (&status) == true);

    printf ("Name: %s Age: %d\n", recover.name, recover.age);

    status = sat_array_get_object_by_parameter (array, "Jane", compare, &recover);
    assert (sat_status_get_result (&status) == true);

    status = sat_array_remove_by (array, 0);
    assert (sat_status_get_result (&status) == true);

    status = sat_array_get_size (array, &size);
    assert (size == 1);

    printf ("Size: %d\n", size);

    status = sat_array_destroy (array);
    assert (sat_status_get_result (&status) == true);

    return 0;
}