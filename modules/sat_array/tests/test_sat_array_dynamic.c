#include <sat.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

/**
 * @brief Tests specific to dynamic arrays
 */

typedef struct
{
    int id;
    double value;
    char label [32];

} dynamic_test_item_t;

void test_dynamic_array_growth (void)
{
    
    sat_array_t *array = NULL;
    sat_array_args_t args =
    {
        .size = 2,  // Small initial capacity to test growth
        .object_size = sizeof (dynamic_test_item_t),
        .mode = sat_array_mode_dynamic
    };
    
    sat_status_t status = sat_array_create (&array, &args);
    assert (sat_status_get_result (&status) == true);
    
    // Verify capacidade inicial
    uint32_t initial_capacity = 0;
    status = sat_array_get_capacity (array, &initial_capacity);
    assert (sat_status_get_result (&status) == true);
    assert (initial_capacity == 2);
    
    // Add items beyond initial capacity
    dynamic_test_item_t items [] =
    {
        { 1, 1.1, "Item 1" },
        { 2, 2.2, "Item 2" },
        { 3, 3.3, "Item 3" },  // This should cause growth
        { 4, 4.4, "Item 4" },
        { 5, 5.5, "Item 5" }   // This may cause another growth
    };
    
    for (int i = 0; i < 5; i++)
    {
        status = sat_array_add (array, &items [i]);
        assert (sat_status_get_result (&status) == true);

        // Verify current size
        uint32_t current_size = 0;
        status = sat_array_get_size (array, &current_size);
        assert (sat_status_get_result (&status) == true);

        // Verify current capacity
        uint32_t current_capacity = 0;
        status = sat_array_get_capacity (array, &current_capacity);
        assert (sat_status_get_result (&status) == true);
    }
    
    // Verify that capacity grew
    uint32_t final_capacity = 0;
    status = sat_array_get_capacity (array, &final_capacity);
    assert (sat_status_get_result (&status) == true);
    assert (final_capacity > initial_capacity);

    // Verify that all items were preserved
    for (int i = 0; i < 5; i++)
    {
        dynamic_test_item_t retrieved_item;
        status = sat_array_get_object_by (array, i, &retrieved_item);
        assert (sat_status_get_result (&status) == true);
        assert (retrieved_item.id == items [i].id);
        assert (retrieved_item.value == items [i].value);
        assert (strcmp (retrieved_item.label, items [i].label) == 0);
    }

    sat_array_destroy (array);
}

void test_dynamic_array_large_scale (void)
{
    
    sat_array_t *array = NULL;
    sat_array_args_t args =
    {
        .size = 1,  // Começar bem pequeno
        .object_size = sizeof (int),
        .mode = sat_array_mode_dynamic
    };

    sat_status_t status = sat_array_create (&array, &args);
    assert (sat_status_get_result (&status) == true);
    
    const int NUM_ITEMS = 1000;
    
    // Add muitos itens para testar múltiplos crescimentos
    for (int i = 0; i < NUM_ITEMS; i++)
    {
        int value = i * 10;
        status = sat_array_add (array, &value);
        assert (sat_status_get_result (&status) == true);

        if (i % 100 == 0 || i < 10)
        {
            uint32_t current_size = 0;
            uint32_t current_capacity = 0;
            sat_array_get_size (array, &current_size);
            sat_array_get_capacity (array, &current_capacity);
        }
    }
    
    // Verify size final
    uint32_t final_size = 0;
    status = sat_array_get_size (array, &final_size);
    assert (sat_status_get_result (&status) == true);
    assert (final_size == NUM_ITEMS);

    // Verify some itens aleatórios
    int indices_to_check [] = {0, 50, 500, 999};
    for (int i = 0; i < 4; i++)
    {
        int index = indices_to_check [i];
        int retrieved_value;
        status = sat_array_get_object_by (array, index, &retrieved_value);
        assert (sat_status_get_result (&status) == true);
        assert (retrieved_value == index * 10);
    }
    
    sat_array_destroy (array);
}

void test_dynamic_array_operations_after_growth (void)
{
    sat_array_t *array = NULL;
    sat_array_args_t args =
    {
        .size = 2,
        .object_size = sizeof (dynamic_test_item_t),
        .mode = sat_array_mode_dynamic
    };

    sat_status_t status = sat_array_create (&array, &args);
    assert (sat_status_get_result (&status) == true);
    
    // Add itens para forçar crescimento
    dynamic_test_item_t items [] =
    {
        { 10, 10.5, "A" },
        { 20, 20.5, "B" },
        { 30, 30.5, "C" },  // Força crescimento
        { 40, 40.5, "D" },
        { 50, 50.5, "E" }
    };
    
    for (int i = 0; i < 5; i++)
    {
        status = sat_array_add (array, &items [i]);
        assert (sat_status_get_result (&status) == true);
    }
    
    // Testar update após crescimento
    dynamic_test_item_t updated_item = { 30, 99.9, "Updated C" };
    status = sat_array_update_by (array, &updated_item, 2);
    assert (sat_status_get_result (&status) == true);

    // Verify update
    dynamic_test_item_t retrieved_item;
    status = sat_array_get_object_by (array, 2, &retrieved_item);
    assert (sat_status_get_result (&status) == true);
    assert (retrieved_item.id == 30);
    assert (retrieved_item.value == 99.9);
    assert (strcmp (retrieved_item.label, "Updated C") == 0);

    // Testar removal após crescimento
    status = sat_array_remove_by (array, 1);  // Remove item B
    assert (sat_status_get_result (&status) == true);

    // Verify que items foram deslocados corretamente
    status = sat_array_get_object_by (array, 1, &retrieved_item);
    assert (sat_status_get_result (&status) == true);
    assert (retrieved_item.id == 30);  // Updated C deve estar no index 1 agora

    // Add mais itens após removal
    dynamic_test_item_t new_item = { 60, 60.6, "F" };
    status = sat_array_add (array, &new_item);
    assert (sat_status_get_result (&status) == true);

    uint32_t final_size = 0;
    status = sat_array_get_size (array, &final_size);
    assert (sat_status_get_result (&status) == true);

    sat_array_destroy (array);
}

void test_dynamic_array_clear_and_reuse (void)
{
    sat_array_t *array = NULL;
    sat_array_args_t args =
    {
        .size = 3,
        .object_size = sizeof (int),
        .mode = sat_array_mode_dynamic
    };

    sat_status_t status = sat_array_create (&array, &args);
    assert (sat_status_get_result (&status) == true);

    // Add itens para forçar crescimento
    for (int i = 0; i < 10; i++)
    {
        int value = i;
        status = sat_array_add (array, &value);
        assert (sat_status_get_result (&status) == true);
    }
    
    uint32_t size_before_clear = 0;
    uint32_t capacity_before_clear = 0;
    sat_array_get_size (array, &size_before_clear);
    sat_array_get_capacity (array, &capacity_before_clear);

    // Clear array
    status = sat_array_clear (array);
    assert (sat_status_get_result (&status) == true);
    
    uint32_t size_after_clear = 0;
    uint32_t capacity_after_clear = 0;
    sat_array_get_size (array, &size_after_clear);
    sat_array_get_capacity (array, &capacity_after_clear);
    assert (size_after_clear == 0);

    // Reutilizar o array
    for (int i = 100; i < 105; i++)
    {
        int value = i;
        status = sat_array_add (array, &value);
        assert (sat_status_get_result (&status) == true);
    }
    
    uint32_t size_after_reuse = 0;
    sat_array_get_size (array, &size_after_reuse);
    assert (size_after_reuse == 5);
    
    // Verify que os novos valores estão corretos
    for (int i = 0; i < 5; i++)
    {
        int retrieved_value;
        status = sat_array_get_object_by (array, i, &retrieved_value);
        assert (sat_status_get_result (&status) == true);
        assert (retrieved_value == 100 + i);
    }

    sat_array_destroy (array);
}

int main(int argc, char *argv [])
{

    test_dynamic_array_growth ();
    test_dynamic_array_large_scale ();
    test_dynamic_array_operations_after_growth ();
    test_dynamic_array_clear_and_reuse ();
    
    return 0;
}
