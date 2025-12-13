#include <stdio.h>
#include <sat_linked_list.h>
#include <sat_iterator.h>
#include <assert.h>
#include <string.h>

typedef struct 
{
    char name [100];
    int age;
} person_t;

bool compare_by_name (const void *const element, const void *const param)
{
    const char *name = (const char *)param;
    const person_t *person = (const person_t *)element;

    return strstr (person->name, name) != NULL;
}

void print_person (const void *const element)
{
    const person_t *person = (const person_t *)element;
    printf ("Name: %s, Age: %d\n", person->name, person->age);
}

void test_linked_list_with_iterator (void);
void test_linked_list_get_ref_by_parameter (void);

int main (int argc, char *argv [])
{
    test_linked_list_with_iterator ();
    test_linked_list_get_ref_by_parameter ();

    return 0;
}

void test_linked_list_with_iterator (void)
{
    sat_linked_list_t *list;
    sat_status_t status;
    sat_iterator_t iterator;
    sat_iterator_base_t *base;
    uint32_t size;

    status = sat_linked_list_create (&list, sizeof(person_t));
    assert (sat_status_get_result (&status) == true);

    person_t john =  {.name = "John Doe",       .age = 35};
    person_t jane =  {.name = "Jane Smith",     .age = 31};
    person_t bob =   {.name = "Bob Johnson",    .age = 42};
    person_t alice = {.name = "Alice Williams", .age = 28};

    status = sat_linked_list_insert (list, &john);
    assert (sat_status_get_result (&status) == true);

    status = sat_linked_list_insert (list, &jane);
    assert (sat_status_get_result (&status) == true);

    status = sat_linked_list_insert (list, &bob);
    assert (sat_status_get_result (&status) == true);

    status = sat_linked_list_insert (list, &alice);
    assert (sat_status_get_result (&status) == true);

    status = sat_linked_list_get_size (list, &size);
    assert (sat_status_get_result (&status) == true);

    base = (sat_iterator_base_t *)list;

    status = sat_iterator_open (&iterator, base);
    assert (sat_status_get_result (&status) == true);

    // Iterate through all elements
    person_t *current;
    int count = 0;

    while ((current = (person_t *)sat_iterator_next (&iterator)) != NULL)
    {
        count++;
        printf ("%d. ", count);
        print_person (current);
    }

    person_t found;
    status = sat_linked_list_get (list, compare_by_name, "Jane", &found);
    if (sat_status_get_result(&status) == true)
    {
        printf ("✓ Found: ");
        print_person (&found);
    }

    status = sat_linked_list_remove (list, compare_by_name, "Bob");
    assert (sat_status_get_result (&status) == true);
    printf ("✓ Removed successfully\n");

    status = sat_iterator_open (&iterator, base);
    assert (sat_status_get_result (&status) == true);
    
    count = 0;
    while ((current = (person_t *)sat_iterator_next (&iterator)) != NULL)
    {
        count++;
        printf ("%d. ", count);
        print_person (current);
    }

    printf ("✓ Now have %d elements\n", count);

    status = sat_linked_list_get_size (list, &size);
    assert (sat_status_get_result (&status) == true);
    printf ("✓ Final list size: %u\n", size);

    status = sat_linked_list_destroy (list);
    assert (sat_status_get_result(&status) == true);
    printf ("\n✓ Linked list destroyed successfully\n");
}


void test_linked_list_get_ref_by_parameter (void)
{
    sat_linked_list_t *list;
    sat_status_t status;

    status = sat_linked_list_create (&list, sizeof(person_t));
    assert (sat_status_get_result (&status) == true);

    person_t john =  {.name = "John Doe",       .age = 35};
    person_t jane =  {.name = "Jane Smith",     .age = 31};
    person_t bob =   {.name = "Bob Johnson",    .age = 42};
    person_t alice = {.name = "Alice Williams", .age = 28};

    status = sat_linked_list_insert (list, &john);
    assert (sat_status_get_result (&status) == true);

    status = sat_linked_list_insert (list, &jane);
    assert (sat_status_get_result (&status) == true);

    status = sat_linked_list_insert (list, &bob);
    assert (sat_status_get_result (&status) == true);

    status = sat_linked_list_insert (list, &alice);
    assert (sat_status_get_result (&status) == true);

    person_t *reference;
    status = sat_linked_list_get_ref (list, compare_by_name, "Alice", (void **)&reference);
    assert (sat_status_get_result (&status) == true);

    reference->age += 1;

    person_t recovered;
    status = sat_linked_list_get (list, compare_by_name, "Alice", &recovered);
    assert (sat_status_get_result (&status) == true);

    assert (recovered.age == 29);

    status = sat_linked_list_destroy (list);
    assert (sat_status_get_result(&status) == true);
}