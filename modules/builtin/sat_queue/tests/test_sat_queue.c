#include <sat.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

typedef struct 
{
    char name [100];
    int age;
} person_t;

static void print (const void *const element)
{
    const person_t *person = (const person_t *)element;
    printf ("Name: %s, Age: %d\n", person->name, person->age);
}

int main (int argc, char *argv[])
{
    uint32_t size = 0;

    person_t john = {.name = "John Doe", .age = 35};
    person_t jane = {.name = "Jane Doe", .age = 31};

    person_t recover;

    sat_queue_t *queue;

    sat_status_t status = sat_queue_create (&queue, sizeof (person_t));
    assert (sat_status_get_result (&status) == true);

    status = sat_queue_enqueue (queue, &john);
    assert (sat_status_get_result (&status) == true);

    status = sat_queue_enqueue (queue, &jane);
    assert (sat_status_get_result (&status) == true);

    status = sat_queue_get_size (queue, &size);
    assert (sat_status_get_result (&status) == true);

    assert (size == 2);

    sat_queue_debug (queue, print);

    status = sat_queue_dequeue (queue,  &recover);
    assert (sat_status_get_result (&status) == true);

    assert (strcmp (recover.name, john.name) == 0);

    status = sat_queue_get_size (queue, &size);
    assert (sat_status_get_result (&status) == true);

    assert (size == 1);

    sat_queue_debug (queue, print);

    status = sat_queue_destroy (queue);
    assert (sat_status_get_result (&status) == true);

    return 0;

}