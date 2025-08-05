#include <sat.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <limits.h>

/**
 * @brief Tests for edge cases and boundary conditions
 */

typedef struct
{
    char data[1];  // Minimal size structure
} tiny_item_t;

typedef struct
{
    char large_data[1024];  // Large structure
    int id;
    double values[100];
} large_item_t;

void test_array_with_minimal_size_objects(void)
{
    
    sat_array_t *array = NULL;
    sat_array_args_t args =
    {
        .size = 5,
        .object_size = sizeof(char),  // Minimum size: 1 byte
        .mode = sat_array_mode_static
    };
    
    sat_status_t status = sat_array_create(&array, &args);
    assert(sat_status_get_result(&status) == true);
    
    // Add some bytes
    char values[] = {'A', 'B', 'C', 'D', 'E'};
    for (int i = 0; i < 5; i++) {
        status = sat_array_add(array, &values[i]);
        assert(sat_status_get_result(&status) == true);
    }
    
    // Verify valores
    for (int i = 0; i < 5; i++) {
        char retrieved_value;
        status = sat_array_get_object_by(array, i, &retrieved_value);
        assert(sat_status_get_result(&status) == true);
        assert(retrieved_value == values[i]);
    }
    
    sat_array_destroy(array);
}

void test_array_with_large_objects(void)
{
    
    sat_array_t *array = NULL;
    sat_array_args_t args = {
        .size = 3,
        .object_size = sizeof(large_item_t),  // Objeto grande
        .mode = sat_array_mode_static
    };
    
    sat_status_t status = sat_array_create(&array, &args);
    assert(sat_status_get_result(&status) == true);
    
    // Create itens grandes
    large_item_t items[3];
    for (int i = 0; i < 3; i++) {
        items[i].id = i + 1000;
        
        // Preencher array de valores
        for (int j = 0; j < 100; j++) {
            items[i].values[j] = (i + 1) * 100.0 + j;
        }
        
        status = sat_array_add(array, &items[i]);
        assert(sat_status_get_result(&status) == true);
    }
    
    // Verify itens grandes
    for (int i = 0; i < 3; i++) {
        large_item_t retrieved_item;
        status = sat_array_get_object_by(array, i, &retrieved_item);
        assert(sat_status_get_result(&status) == true);
        assert(retrieved_item.id == items[i].id);
        assert(strcmp(retrieved_item.large_data, items[i].large_data) == 0);
        
        // Verify some valores do array interno
        for (int j = 0; j < 10; j++) {
            assert(retrieved_item.values[j] == items[i].values[j]);
        }
    }
    
    sat_array_destroy(array);
}

void test_array_single_element_operations(void)
{
    
    sat_array_t *array = NULL;
    sat_array_args_t args = {
        .size = 1,  // Array com capacidade para apenas 1 elemento
        .object_size = sizeof(int),
        .mode = sat_array_mode_static
    };
    
    sat_status_t status = sat_array_create(&array, &args);
    assert(sat_status_get_result(&status) == true);
    
    // Verify estado inicial
    uint32_t size = 0;
    status = sat_array_get_size(array, &size);
    assert(sat_status_get_result(&status) == true);
    assert(size == 0);
    
    // Add um elemento
    int value = 42;
    status = sat_array_add(array, &value);
    assert(sat_status_get_result(&status) == true);
    
    // Verify que array está cheio
    status = sat_array_get_size(array, &size);
    assert(sat_status_get_result(&status) == true);
    assert(size == 1);
    
    // Tentar adicionar outro elemento (deve falhar)
    int second_value = 99;
    status = sat_array_add(array, &second_value);
    assert(sat_status_get_result(&status) == false);
    
    // Atualizar o único elemento
    int updated_value = 100;
    status = sat_array_update_by(array, &updated_value, 0);
    assert(sat_status_get_result(&status) == true);
    
    // Verify update
    int retrieved_value;
    status = sat_array_get_object_by(array, 0, &retrieved_value);
    assert(sat_status_get_result(&status) == true);
    assert(retrieved_value == updated_value);
    
    // Remover o único elemento
    status = sat_array_remove_by(array, 0);
    assert(sat_status_get_result(&status) == true);
    
    // Verify que array está vazio
    status = sat_array_get_size(array, &size);
    assert(sat_status_get_result(&status) == true);
    assert(size == 0);
    
    // Tentar remover de array vazio (deve falhar)
    status = sat_array_remove_by(array, 0);
    assert(sat_status_get_result(&status) == false);
    
    sat_array_destroy(array);
}

void test_array_boundary_indices(void)
{
    
    sat_array_t *array = NULL;
    sat_array_args_t args = {
        .size = 5,
        .object_size = sizeof(int),
        .mode = sat_array_mode_static
    };
    
    sat_status_t status = sat_array_create(&array, &args);
    assert(sat_status_get_result(&status) == true);
    
    // Add some elementos
    int values[] = {10, 20, 30};
    for (int i = 0; i < 3; i++) {
        status = sat_array_add(array, &values[i]);
        assert(sat_status_get_result(&status) == true);
    }
    
    // Test com index 0 (primeiro elemento)
    int retrieved_value;
    status = sat_array_get_object_by(array, 0, &retrieved_value);
    assert(sat_status_get_result(&status) == true);
    assert(retrieved_value == 10);
    
    // Test com último index válido
    status = sat_array_get_object_by(array, 2, &retrieved_value);
    assert(sat_status_get_result(&status) == true);
    assert(retrieved_value == 30);
    
    // Test com index igual ao size (inválido)
    status = sat_array_get_object_by(array, 3, &retrieved_value);
    assert(sat_status_get_result(&status) == false);
    
    // Test com index muito grande
    status = sat_array_get_object_by(array, UINT32_MAX, &retrieved_value);
    assert(sat_status_get_result(&status) == false);
    
    // Test update com indexs limite
    int new_value = 999;
    status = sat_array_update_by(array, &new_value, 0);
    assert(sat_status_get_result(&status) == true);
    
    status = sat_array_update_by(array, &new_value, 2);
    assert(sat_status_get_result(&status) == true);
    
    status = sat_array_update_by(array, &new_value, 3);
    assert(sat_status_get_result(&status) == false);
    
    // Test removal com indexs limite
    status = sat_array_remove_by(array, 2);  // Remove último elemento
    assert(sat_status_get_result(&status) == true);
    
    status = sat_array_remove_by(array, 2);  // Agora inválido
    assert(sat_status_get_result(&status) == false);
    
    sat_array_destroy(array);
}

void test_array_operations_on_uninitialized(void)
{
    
    // Todas as operações com array NULL devem falhar
    sat_status_t status;
    int dummy_value = 42;
    uint32_t dummy_size;
    
    status = sat_array_add(NULL, &dummy_value);
    assert(sat_status_get_result(&status) == false);
    
    status = sat_array_get_object_by(NULL, 0, &dummy_value);
    assert(sat_status_get_result(&status) == false);
    
    status = sat_array_update_by(NULL, &dummy_value, 0);
    assert(sat_status_get_result(&status) == false);
    
    status = sat_array_remove_by(NULL, 0);
    assert(sat_status_get_result(&status) == false);
    
    status = sat_array_get_size(NULL, &dummy_size);
    assert(sat_status_get_result(&status) == false);
    
    status = sat_array_get_capacity(NULL, &dummy_size);
    assert(sat_status_get_result(&status) == false);
    
    status = sat_array_clear(NULL);
    assert(sat_status_get_result(&status) == false);
    
    void *ref = sat_array_get_reference_by(NULL, 0);
    assert(ref == NULL);
    
    status = sat_array_destroy(NULL);
    assert(sat_status_get_result(&status) == false);
    
}

void test_array_stress_operations() {
    
    sat_array_t *array = NULL;
    sat_array_args_t args = {
        .size = 10,
        .object_size = sizeof(int),
        .mode = sat_array_mode_static
    };
    
    sat_status_t status = sat_array_create(&array, &args);
    assert(sat_status_get_result(&status) == true);
    
    const int NUM_CYCLES = 100;
    
    // Ciclo de operações intensivas
    for (int cycle = 0; cycle < NUM_CYCLES; cycle++) {
        // Preencher array
        for (int i = 0; i < 10; i++) {
            int value = cycle * 10 + i;
            status = sat_array_add(array, &value);
            assert(sat_status_get_result(&status) == true);
        }
        
        // Verify all elementos
        for (int i = 0; i < 10; i++) {
            int retrieved_value;
            status = sat_array_get_object_by(array, i, &retrieved_value);
            assert(sat_status_get_result(&status) == true);
            assert(retrieved_value == cycle * 10 + i);
        }
        
        // Atualizar elementos pares
        for (int i = 0; i < 10; i += 2) {
            int new_value = -(cycle * 10 + i);
            status = sat_array_update_by(array, &new_value, i);
            assert(sat_status_get_result(&status) == true);
        }
        
        // Verify atualizações
        for (int i = 0; i < 10; i += 2) {
            int retrieved_value;
            status = sat_array_get_object_by(array, i, &retrieved_value);
            assert(sat_status_get_result(&status) == true);
            assert(retrieved_value == -(cycle * 10 + i));
        }
        
        // Clear array para próximo ciclo
        status = sat_array_clear(array);
        assert(sat_status_get_result(&status) == true);
        
        uint32_t size;
        status = sat_array_get_size(array, &size);
        assert(sat_status_get_result(&status) == true);
        assert(size == 0);
        
        if (cycle % 20 == 0) {
        }
    }
    
    
    sat_array_destroy(array);
}

int main(int argc, char *argv[]) {
    
    test_array_with_minimal_size_objects();
    test_array_with_large_objects();
    test_array_single_element_operations();
    test_array_boundary_indices();
    test_array_operations_on_uninitialized();
    test_array_stress_operations();
    
    return 0;
}
