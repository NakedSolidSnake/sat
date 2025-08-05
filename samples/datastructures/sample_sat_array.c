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

void test_static (void);
void test_dynamic (void);

int main (int argc, char *argv[])
{
    test_static ();
    test_dynamic ();

    return 0;
}

void test_static (void)
{
    uint32_t size;

    person_t john = {.name = "John Doe", .age = 35};
    person_t jane = {.name = "Jane Doe", .age = 31};

    person_t recover;

    sat_array_t *array;

    sat_status_t status = sat_array_create (&array, &(sat_array_args_t)
                                                    {
                                                        .size = 2,
                                                        .object_size = sizeof (person_t),
                                                        .mode = sat_array_mode_static,
                                                    });
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
}

void test_dynamic (void)
{
    uint32_t size;
    uint32_t capacity;

    person_t john = {.name = "John Doe", .age = 35};
    person_t jane = {.name = "Jane Doe", .age = 31};

    person_t recover;

    sat_array_t *array;

    sat_status_t status = sat_array_create (&array, &(sat_array_args_t)
                                                    {
                                                        .size = 1,
                                                        .object_size = sizeof (person_t),
                                                        .mode = sat_array_mode_dynamic,
                                                    });
    assert (sat_status_get_result (&status) == true);

    status = sat_array_add (array, &john);
    assert (sat_status_get_result (&status) == true);

    status = sat_array_get_capacity (array, &capacity);
    assert (capacity == 1);

    printf ("Capacity: %d\n", capacity);

    status = sat_array_add (array, &jane);
    assert (sat_status_get_result (&status) == true);

    status = sat_array_get_capacity (array, &capacity);
    assert (capacity == 2);

    printf ("Capacity: %d\n", capacity);

    status = sat_array_add (array, &jane);
    assert (sat_status_get_result (&status) == true);

    status = sat_array_get_capacity (array, &capacity);
    assert (capacity == 4);

    printf ("Capacity: %d\n", capacity);

    status = sat_array_get_size (array, &size);
    assert (size == 3);

    printf ("Size: %d\n", size);

    status = sat_array_get_object_by (array, 0, &recover);
    assert (sat_status_get_result (&status) == true);

    printf ("Name: %s Age: %d\n", recover.name, recover.age);

    status = sat_array_get_object_by_parameter (array, "Jane", compare, &recover);
    assert (sat_status_get_result (&status) == true);

    status = sat_array_remove_by (array, 0);
    assert (sat_status_get_result (&status) == true);

    status = sat_array_get_size (array, &size);
    assert (size == 2);

    printf ("Size: %d\n", size);

    status = sat_array_destroy (array);
    assert (sat_status_get_result (&status) == true);
}