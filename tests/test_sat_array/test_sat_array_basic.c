#include <sat.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#define NAME_SIZE 32

/**
 * @brief Basic tests for sat_array creation and destruction
 */

typedef struct
{
    int id;
    char name [NAME_SIZE];
    float value;
} test_item_t;

void test_array_creation_and_destruction (void)
{
    sat_array_t *array = NULL;
    sat_array_args_t args =
    {
        .size = 10,
        .object_size = sizeof (test_item_t),
        .mode = sat_array_mode_static
    };
    
    sat_status_t status = sat_array_create (&array, &args);
    assert (sat_status_get_result (&status) == true);
    assert (array != NULL);
    
    uint32_t capacity = 0;
    status = sat_array_get_capacity (array, &capacity);
    assert (sat_status_get_result (&status) == true);
    assert (capacity == 10);
    
    uint32_t size = 0;
    status = sat_array_get_size (array, &size);
    assert (sat_status_get_result (&status) == true);
    assert (size == 0);
    
    status = sat_array_destroy (array);
    assert (sat_status_get_result (&status) == true);
}

void test_array_creation_with_invalid_args (void)
{
    sat_array_t *array = NULL;
    sat_status_t status;
    
    // Test with NULL args
    status = sat_array_create (&array, NULL);
    assert (sat_status_get_result (&status) == false);
    
    // Test with NULL array pointer
    sat_array_args_t args =
    {
        .size = 10,
        .object_size = sizeof (test_item_t),
        .mode = sat_array_mode_static
    };

    status = sat_array_create (NULL, &args);
    assert (sat_status_get_result (&status) == false);
    
    // Test with zero size
    args.size = 0;
    status = sat_array_create (&array, &args);
    assert (sat_status_get_result (&status) == false);
    
    // Test with zero object_size
    args.size = 10;
    args.object_size = 0;
    status = sat_array_create (&array, &args);
    assert (sat_status_get_result (&status) == false);
}

void test_array_modes (void)
{
    sat_array_t *static_array = NULL;
    sat_array_t *dynamic_array = NULL;
    
    // Create static array
    sat_array_args_t static_args =
    {
        .size = 5,
        .object_size = sizeof (int),
        .mode = sat_array_mode_static
    };

    sat_status_t status = sat_array_create (&static_array, &static_args);
    assert (sat_status_get_result (&status) == true);
    
    // Create dynamic array
    sat_array_args_t dynamic_args =
    {
        .size = 5,
        .object_size = sizeof (int),
        .mode = sat_array_mode_dynamic
    };

    status = sat_array_create (&dynamic_array, &dynamic_args);
    assert (sat_status_get_result (&status) == true);
    
    // Clean up
    sat_array_destroy (static_array);
    sat_array_destroy (dynamic_array);
}

void test_array_basic_operations (void)
{
    sat_array_t *array = NULL;
    sat_array_args_t args =
    {
        .size = 5,
        .object_size = sizeof (test_item_t),
        .mode = sat_array_mode_static
    };
    
    sat_status_t status = sat_array_create (&array, &args);
    assert (sat_status_get_result (&status) == true);
    
    test_item_t items [] =
    {
        { 1, "Item 1", 10.5f },
        { 2, "Item 2", 20.3f },
        { 3, "Item 3", 30.7f }
    };
    
    for (int i = 0; i < 3; i++)
    {
        status = sat_array_add (array, &items [i]);
        assert (sat_status_get_result (&status) == true);
    }

    uint32_t size = 0;
    status = sat_array_get_size (array, &size);
    assert (sat_status_get_result (&status) == true);
    assert (size == 3);

    test_item_t retrieved_item;

    for (int i = 0; i < 3; i++)
    {
        status = sat_array_get_object_by (array, i, &retrieved_item);
        assert (sat_status_get_result (&status) == true);
        assert (retrieved_item.id == items [i].id);
        assert (strcmp (retrieved_item.name, items [i].name) == 0);
        assert (retrieved_item.value == items [i].value);
    }
    
    sat_array_destroy (array);
}

int main (int argc, char *argv [])
{
    test_array_creation_and_destruction ();
    test_array_creation_with_invalid_args ();
    test_array_modes ();
    test_array_basic_operations ();

    return 0;
}
