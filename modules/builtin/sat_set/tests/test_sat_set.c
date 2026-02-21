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

// Compare elements by searching the name field for a substring
static bool compare_by_name (const void *const element, const void *const param)
{
    const person_t *person = (const person_t *)element;
    const char *name       = (const char *)param;

    return strstr (person->name, name) != NULL;
}

// Compare elements by matching the id field exactly
static bool compare_by_id (const void *const element, const void *const param)
{
    const person_t *person = (const person_t *)element;
    const int *id          = (const int *)param;

    return person->id == *id;
}

// Equality function used to enforce set uniqueness (by id)
static bool is_equal (const void *const element, const void *const new_element)
{
    const person_t *a = (const person_t *)element;
    const person_t *b = (const person_t *)new_element;

    return a->id == b->id;
}

// Helper: create a fresh static set with capacity 5
static sat_set_t *make_set (void)
{
    sat_set_t *set    = NULL;
    sat_status_t status = sat_set_create (&set, &(sat_set_args_t)
    {
        .size        = 5,
        .object_size = sizeof (person_t),
        .is_equal    = is_equal,
        .mode        = sat_set_mode_static,
    });
    assert (sat_status_get_result (&status) == true);
    return set;
}

// Test: create and destroy
static void test_create_destroy (void)
{
    sat_set_t *set = make_set ();

    sat_status_t status = sat_set_destroy (set);
    assert (sat_status_get_result (&status) == true);
}

// Test: add elements and reject duplicates
static void test_add_and_uniqueness (void)
{
    sat_set_t *set = make_set ();
    uint32_t size  = 0;

    person_t john = {.id = 1, .name = "John Doe", .age = 35};
    person_t jane = {.id = 2, .name = "Jane Doe", .age = 31};

    sat_status_t status = sat_set_add (set, &john);
    assert (sat_status_get_result (&status) == true);

    status = sat_set_add (set, &jane);
    assert (sat_status_get_result (&status) == true);

    // Duplicate must be rejected
    status = sat_set_add (set, &jane);
    assert (sat_status_get_result (&status) == false);

    sat_set_get_size (set, &size);
    assert (size == 2);

    sat_set_destroy (set);
}

// Test: get element by index
static void test_get_object_by (void)
{
    sat_set_t *set    = make_set ();
    person_t john    = {.id = 1, .name = "John Doe", .age = 35};
    person_t recover = {0};

    sat_set_add (set, &john);

    sat_status_t status = sat_set_get_object_by (set, 0, &recover);
    assert (sat_status_get_result (&status) == true);
    assert (recover.id == 1);
    assert (strcmp (recover.name, "John Doe") == 0);

    // Out-of-bounds index must fail
    status = sat_set_get_object_by (set, 99, &recover);
    assert (sat_status_get_result (&status) == false);

    sat_set_destroy (set);
}

// Test: get element by parameter
static void test_get_object_by_parameter (void)
{
    sat_set_t *set = make_set ();
    person_t jane  = {.id = 2, .name = "Jane Doe", .age = 31};
    person_t found = {0};

    sat_set_add (set, &jane);

    sat_status_t status = sat_set_get_object_by_parameter (set, "Jane", compare_by_name, &found);
    assert (sat_status_get_result (&status) == true);
    assert (found.id == 2);

    // Non-existing parameter must fail
    status = sat_set_get_object_by_parameter (set, "Nobody", compare_by_name, &found);
    assert (sat_status_get_result (&status) == false);

    sat_set_destroy (set);
}

// Test: update element by index
static void test_update_by (void)
{
    sat_set_t *set    = make_set ();
    person_t original = {.id = 1, .name = "John Doe", .age = 35};
    person_t updated  = {.id = 1, .name = "John Smith", .age = 36};
    person_t recover  = {0};

    sat_set_add (set, &original);

    sat_status_t status = sat_set_update_by (set, &updated, 0);
    assert (sat_status_get_result (&status) == true);

    sat_set_get_object_by (set, 0, &recover);
    assert (strcmp (recover.name, "John Smith") == 0);
    assert (recover.age == 36);

    // Out-of-bounds index must fail
    status = sat_set_update_by (set, &updated, 99);
    assert (sat_status_get_result (&status) == false);

    sat_set_destroy (set);
}

// Test: remove element by index
static void test_remove_by (void)
{
    sat_set_t *set = make_set ();
    uint32_t size  = 0;

    person_t john = {.id = 1, .name = "John Doe", .age = 35};
    person_t jane = {.id = 2, .name = "Jane Doe", .age = 31};

    sat_set_add (set, &john);
    sat_set_add (set, &jane);

    sat_status_t status = sat_set_remove_by (set, 0);
    assert (sat_status_get_result (&status) == true);

    sat_set_get_size (set, &size);
    assert (size == 1);

    // Verify remaining element is Jane
    person_t recover = {0};
    sat_set_get_object_by (set, 0, &recover);
    assert (recover.id == 2);

    sat_set_destroy (set);
}

// Test: remove element by parameter
static void test_remove_by_parameter (void)
{
    sat_set_t *set = make_set ();
    uint32_t size  = 0;

    person_t john = {.id = 1, .name = "John Doe", .age = 35};
    person_t jane = {.id = 2, .name = "Jane Doe", .age = 31};

    sat_set_add (set, &john);
    sat_set_add (set, &jane);

    person_t removed    = {0};
    int search_id       = 1;
    sat_status_t status = sat_set_remove_by_parameter (set, &search_id, compare_by_id, &removed);
    assert (sat_status_get_result (&status) == true);
    assert (removed.id == 1);

    sat_set_get_size (set, &size);
    assert (size == 1);

    // Non-existing element must fail
    status = sat_set_remove_by_parameter (set, &search_id, compare_by_id, &removed);
    assert (sat_status_get_result (&status) == false);

    sat_set_destroy (set);
}

// Test: zero-copy reference access
static void test_get_object_ref_by_parameter (void)
{
    sat_set_t *set = make_set ();
    person_t john  = {.id = 1, .name = "John Doe", .age = 35};

    sat_set_add (set, &john);

    person_t *ref       = NULL;
    int search_id       = 1;
    sat_status_t status = sat_set_get_object_ref_by_parameter (
        set, &search_id, compare_by_id, (void **)&ref);
    assert (sat_status_get_result (&status) == true);
    assert (ref != NULL);
    assert (ref->id == 1);

    // Modify through the reference and verify the change is visible in the set
    ref->age = 99;
    person_t recover = {0};
    sat_set_get_object_by (set, 0, &recover);
    assert (recover.age == 99);

    // Non-existing element must fail
    int missing_id = 999;
    status = sat_set_get_object_ref_by_parameter (
        set, &missing_id, compare_by_id, (void **)&ref);
    assert (sat_status_get_result (&status) == false);

    sat_set_destroy (set);
}

// Test: clone a set
static void test_clone (void)
{
    sat_set_t *original = make_set ();
    sat_set_t *cloned   = NULL;
    uint32_t size       = 0;

    person_t john = {.id = 1, .name = "John Doe", .age = 35};
    person_t jane = {.id = 2, .name = "Jane Doe", .age = 31};

    sat_set_add (original, &john);
    sat_set_add (original, &jane);

    sat_status_t status = sat_set_clone (original, &cloned);
    assert (sat_status_get_result (&status) == true);
    assert (cloned != NULL);

    // Cloned set must have the same number of elements
    sat_set_get_size (cloned, &size);
    assert (size == 2);

    // Cloned elements must match the originals
    person_t recover = {0};
    sat_set_get_object_by (cloned, 0, &recover);
    assert (recover.id == 1);
    assert (strcmp (recover.name, "John Doe") == 0);

    sat_set_get_object_by (cloned, 1, &recover);
    assert (recover.id == 2);
    assert (strcmp (recover.name, "Jane Doe") == 0);

    // Cloned set must be independent: modifying the clone must not affect the original
    person_t updated = {.id = 1, .name = "Jane Clone", .age = 99};
    sat_set_update_by (cloned, &updated, 0);

    person_t original_elem = {0};
    sat_set_get_object_by (original, 0, &original_elem);
    assert (strcmp (original_elem.name, "John Doe") == 0);

    // Duplicates must still be rejected in the cloned set
    status = sat_set_add (cloned, &jane);
    assert (sat_status_get_result (&status) == false);

    sat_set_destroy (original);
    sat_set_destroy (cloned);
}

// Test: dynamic mode grows automatically
static void test_dynamic_mode (void)
{
    sat_set_t *set    = NULL;
    sat_status_t status = sat_set_create (&set, &(sat_set_args_t)
    {
        .size        = 2,
        .object_size = sizeof (person_t),
        .is_equal    = is_equal,
        .mode        = sat_set_mode_dynamic,
    });
    assert (sat_status_get_result (&status) == true);

    // Add more elements than the initial capacity
    for (int i = 1; i <= 5; i++)
    {
        person_t p = {.id = i, .age = i * 10};
        snprintf (p.name, sizeof (p.name), "Person %d", i);

        status = sat_set_add (set, &p);
        assert (sat_status_get_result (&status) == true);
    }

    uint32_t size = 0;
    sat_set_get_size (set, &size);
    assert (size == 5);

    sat_set_destroy (set);
}

int main (int argc, char *argv[])
{
    test_create_destroy ();
    test_add_and_uniqueness ();
    test_get_object_by ();
    test_get_object_by_parameter ();
    test_update_by ();
    test_remove_by ();
    test_remove_by_parameter ();
    test_get_object_ref_by_parameter ();
    test_clone ();
    test_dynamic_mode ();

    return 0;
}