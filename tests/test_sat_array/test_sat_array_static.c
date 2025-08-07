#include <sat.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

/**
 * @brief Tests specific to static arrays
 */

typedef struct
{
    int id;
    char description [64];

} static_test_item_t;

void test_static_array_capacity_limit (void)
{
    sat_array_t *array = NULL;
    sat_array_args_t args =
    {
        .size = 3,  // Maximum capacity of 3 elements
        .object_size = sizeof (static_test_item_t),
        .mode = sat_array_mode_static
    };

    sat_status_t status = sat_array_create (&array, &args);
    assert (sat_status_get_result (&status) == true);
    
    static_test_item_t items [] =
    {
        { 1, "First item"  },
        { 2, "Second item" },
        { 3, "Third item"  },
        { 4, "should fail" }  // This should fail
    };
    
    // Add items up to limit
    for (int i = 0; i < 3; i++)
    {
        status = sat_array_add (array, &items [i]);
        assert (sat_status_get_result (&status) == true);
    }
    
    // Verify that array is full
    uint32_t size = 0;
    status = sat_array_get_size (array, &size);
    assert (sat_status_get_result (&status) == true);
    assert (size == 3);

    // Try to add fourth item (should fail)
    status = sat_array_add (array, &items [3]);
    assert (sat_status_get_result (&status) == false);
    
    // Verify that size did not change
    status = sat_array_get_size (array, &size);
    assert (sat_status_get_result (&status) == true);
    assert (size == 3);

    sat_array_destroy (array);
}

void test_static_array_operations (void)
{
    sat_array_t *array = NULL;
    sat_array_args_t args =
    {
        .size = 5,
        .object_size = sizeof (static_test_item_t),
        .mode = sat_array_mode_static
    };

    sat_status_t status = sat_array_create (&array, &args);
    assert (sat_status_get_result (&status) == true);
    
    // Add some items
    static_test_item_t items [] =
    {
        { 100, "Item A" },
        { 200, "Item B" },
        { 300, "Item C" }
    };
    
    for (int i = 0; i < 3; i++)
    {
        status = sat_array_add (array, &items [i]);
        assert (sat_status_get_result (&status) == true);
    }
    
    // Test update
    static_test_item_t updated_item = { 200, "Updated Item B" };
    status = sat_array_update_by (array, &updated_item, 1);
    assert (sat_status_get_result (&status) == true);
    
    // Verify if update worked
    static_test_item_t retrieved_item;
    status = sat_array_get_object_by (array, 1, &retrieved_item);
    assert (sat_status_get_result (&status) == true);
    assert (retrieved_item.id == 200);
    assert (strcmp (retrieved_item.description, "Updated Item B") == 0);

    // Test removal
    status = sat_array_remove_by (array, 1);  // Remove "Updated Item B"
    assert (sat_status_get_result (&status) == true);

    // Verify that size decreased
    uint32_t size = 0;
    status = sat_array_get_size (array, &size);
    assert (sat_status_get_result (&status) == true);
    assert (size == 2);
    
    // Verify that items were shifted
    status = sat_array_get_object_by (array, 1, &retrieved_item);
    assert (sat_status_get_result (&status) == true);
    assert (retrieved_item.id == 300);  // "Item C" should have moved to index 1
    assert (strcmp (retrieved_item.description, "Item C") == 0);

    sat_array_destroy (array);
}

void test_static_array_boundary_conditions (void)
{
    sat_array_t *array = NULL;
    sat_array_args_t args =
    {
        .size = 2,
        .object_size = sizeof (int),
        .mode = sat_array_mode_static
    };
    
    sat_status_t status = sat_array_create (&array, &args);
    assert (sat_status_get_result (&status) == true);

    // Tests with empty array
    int retrieved_value;
    status = sat_array_get_object_by (array, 0, &retrieved_value);
    assert (sat_status_get_result (&status) == false);

    status = sat_array_remove_by (array, 0);
    assert (sat_status_get_result (&status) == false);

    status = sat_array_update_by (array, &retrieved_value, 0);
    assert (sat_status_get_result (&status) == false);

    // Add one item
    int value = 42;
    status = sat_array_add (array, &value);
    assert (sat_status_get_result (&status) == true);

    // Test out-of-bounds indexes
    status = sat_array_get_object_by (array, 1, &retrieved_value);
    assert (sat_status_get_result (&status) == false);

    status = sat_array_update_by (array, &value, 1);
    assert (sat_status_get_result (&status) == false);

    status = sat_array_remove_by (array, 1);
    assert (sat_status_get_result (&status) == false);

    // Test with NULL data
    status = sat_array_add (array, NULL);
    assert (sat_status_get_result (&status) == false);

    status = sat_array_update_by (array, NULL, 0);
    assert (sat_status_get_result (&status) == false);

    status = sat_array_get_object_by (array, 0, NULL);
    assert (sat_status_get_result (&status) == false);

    sat_array_destroy (array);
}

void test_static_array_clear (void)
{
    sat_array_t *array = NULL;
    sat_array_args_t args =
    {
        .size = 5,
        .object_size = sizeof (int),
        .mode = sat_array_mode_static
    };

    sat_status_t status = sat_array_create (&array, &args);
    assert (sat_status_get_result (&status) == true);
    
    // Add some items
    int values [] = { 10, 20, 30, 40 };
    for (int i = 0; i < 4; i++)
    {
        status = sat_array_add (array, &values [i]);
        assert (sat_status_get_result (&status) == true);
    }
    
    // Verify size before clear
    uint32_t size = 0;
    status = sat_array_get_size (array, &size);
    assert (sat_status_get_result (&status) == true);
    assert (size == 4);

    // Clear array
    status = sat_array_clear (array);
    assert (sat_status_get_result (&status) == true);
    
    // Verify size after clear
    status = sat_array_get_size (array, &size);
    assert (sat_status_get_result (&status) == true);
    assert (size == 0);

    // Verify that can still add items after clear
    int new_value = 99;
    status = sat_array_add (array, &new_value);
    assert (sat_status_get_result (&status) == true);

    sat_array_destroy (array);
}

int main (int argc, char *argv [])
{
    test_static_array_capacity_limit ();
    test_static_array_operations ();
    test_static_array_boundary_conditions ();
    test_static_array_clear ();

    return 0;
}
