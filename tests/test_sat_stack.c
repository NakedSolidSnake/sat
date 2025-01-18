#include <sat.h>
#include <stdio.h>
#include <assert.h>

typedef struct 
{
    char name [100];
    int age;
} person_t;

int main (int argc, char *argv[])
{
    uint32_t size = 0;

    person_t john = {.name = "John Doe", .age = 35};
    person_t jane = {.name = "Jane Doe", .age = 31};

    sat_stack_t *stack;

    sat_status_t status = sat_stack_create (&stack, 2, sizeof (person_t));
    assert (sat_status_get_result (&status) == true);

    status = sat_stack_push (stack, &john);
    assert (sat_status_get_result (&status) == true);

    status = sat_stack_push (stack, &jane);
    assert (sat_status_get_result (&status) == true);

    status = sat_stack_push (stack, &jane);
    assert (sat_status_get_result (&status) == false);

    status = sat_stack_get_size (stack, &size);
    assert (size == 2);

    printf ("Size: %d\n", size);

    person_t recover;

    status = sat_stack_pop (stack, &recover);
    assert (sat_status_get_result (&status) == true);

    printf ("Name : %s, Age: %d\n", recover.name, recover.age);

    status = sat_stack_pop (stack, &recover);
    assert (sat_status_get_result (&status) == true);

    printf ("Name : %s, Age: %d\n", recover.name, recover.age);

    status = sat_stack_get_size (stack, &size);
    assert (size == 0);

    status = sat_stack_pop (stack, &recover);
    assert (sat_status_get_result (&status) == false);

    status = sat_stack_destroy (stack);
    assert (sat_status_get_result (&status) == true);

    return 0;
}