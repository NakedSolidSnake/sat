#include <sat.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

/**
 * @brief Tests for sat_array iterator functionality
 */

typedef struct {
    int id;
    char name[16];
    float value;
} iter_test_item_t;

void test_array_iterator_basic() {
    
    sat_array_t *array = NULL;
    sat_array_args_t args = {
        .size = 5,
        .object_size = sizeof(iter_test_item_t),
        .mode = sat_array_mode_static
    };
    
    sat_status_t status = sat_array_create(&array, &args);
    assert(sat_status_get_result(&status) == true);
    
    // Add some itens
    iter_test_item_t items[] = {
        {1, "Item1", 10.5f},
        {2, "Item2", 20.3f},
        {3, "Item3", 30.7f},
        {4, "Item4", 40.1f}
    };
    
    for (int i = 0; i < 4; i++) {
        status = sat_array_add(array, &items[i]);
        assert(sat_status_get_result(&status) == true);
    }
    
    // Get o iterador base do array
    sat_iterator_base_t *base = (sat_iterator_base_t *)array;
    
    // Create iterador
    sat_iterator_t iterator;
    status = sat_iterator_open(&iterator, base);
    assert(sat_status_get_result(&status) == true);
    
    // Iterate through all elements
    int count = 0;

    iter_test_item_t *current_item = (iter_test_item_t *)sat_iterator_next(&iterator);

    while (current_item != NULL) {
        
        assert(current_item != NULL);
        
        // Verify que o item corresponde ao esperado
        assert(current_item->id == items[count].id);
        assert(strcmp(current_item->name, items[count].name) == 0);
        assert(current_item->value == items[count].value);
        
        count++;
        current_item = (iter_test_item_t *)sat_iterator_next(&iterator);
    }
    
    assert(count == 4);
    
    // Verify that there are no more elements
    current_item = (iter_test_item_t *)sat_iterator_next(&iterator);
    assert(current_item == NULL);
    
    sat_array_destroy(array);
}

void test_array_iterator_empty_array() {
    
    sat_array_t *array = NULL;
    sat_array_args_t args = {
        .size = 5,
        .object_size = sizeof(int),
        .mode = sat_array_mode_static
    };
    
    sat_status_t status = sat_array_create(&array, &args);
    assert(sat_status_get_result(&status) == true);
    
    // Array está vazio, não adicionar nenhum elemento
    uint32_t size = 0;
    status = sat_array_get_size(array, &size);
    assert(sat_status_get_result(&status) == true);
    assert(size == 0);
    
    // Create iterador para array vazio
    sat_iterator_base_t *base = (sat_iterator_base_t *)array;
    sat_iterator_t iterator;
    status = sat_iterator_open(&iterator, base);
    assert(sat_status_get_result(&status) == true);
    
    // Verify que não há elementos para iterar
    assert(sat_iterator_next(&iterator) == NULL);
    
    sat_array_destroy(array);
}

void test_array_iterator_single_element() {
    
    sat_array_t *array = NULL;
    sat_array_args_t args = {
        .size = 5,
        .object_size = sizeof(int),
        .mode = sat_array_mode_static
    };
    
    sat_status_t status = sat_array_create(&array, &args);
    assert(sat_status_get_result(&status) == true);
    
    // Add apenas um elemento
    int single_value = 42;
    status = sat_array_add(array, &single_value);
    assert(sat_status_get_result(&status) == true);
    
    // Create iterador
    sat_iterator_base_t *base = (sat_iterator_base_t *)array;
    sat_iterator_t iterator;
    status = sat_iterator_open (&iterator, base);
    assert(sat_status_get_result(&status) == true);
    
    // Get o elemento
    int *retrieved_value = (int *)sat_iterator_next(&iterator);
    assert(retrieved_value != NULL);
    assert(*retrieved_value == single_value);
    
    // Verify que não há mais elementos
    assert(sat_iterator_next(&iterator) == NULL);
    
    // Tentar obter próximo elemento (deve retornar NULL)
    void *next_item = sat_iterator_next(&iterator);
    assert(next_item == NULL);
    
    sat_array_destroy(array);
}

void test_array_iterator_modification_during_iteration() {
    
    sat_array_t *array = NULL;
    sat_array_args_t args = {
        .size = 8,
        .object_size = sizeof(int),
        .mode = sat_array_mode_static
    };
    
    sat_status_t status = sat_array_create(&array, &args);
    assert(sat_status_get_result(&status) == true);
    
    // Add some elementos
    int initial_values[] = {10, 20, 30, 40, 50};
    for (int i = 0; i < 5; i++) {
        status = sat_array_add(array, &initial_values[i]);
        assert(sat_status_get_result(&status) == true);
    }
    
    // Create iterador
    sat_iterator_base_t *base = (sat_iterator_base_t *)array;
    sat_iterator_t iterator;
    status = sat_iterator_open(&iterator, base);
    assert(sat_status_get_result(&status) == true);
    
    // Iterar e modificar elementos através de referências
    int count = 0;

    int *current_item = (int *)sat_iterator_next(&iterator);

    while (current_item != NULL) {
        
        assert(current_item != NULL);
        
        
        // Modificar o elemento atual
        *current_item *= 2;
        
        count++;

        current_item = (int *)sat_iterator_next(&iterator);
    }
    
    
    // Verify que as modificações foram aplicadas
    for (int i = 0; i < 5; i++) {
        int retrieved_value;
        status = sat_array_get_object_by(array, i, &retrieved_value);
        assert(sat_status_get_result(&status) == true);
        assert(retrieved_value == initial_values[i] * 2);
    }
    
    sat_array_destroy(array);
}

void test_array_iterator_multiple_iterators() {
    
    sat_array_t *array = NULL;
    sat_array_args_t args = {
        .size = 6,
        .object_size = sizeof(int),
        .mode = sat_array_mode_static
    };
    
    sat_status_t status = sat_array_create(&array, &args);
    assert(sat_status_get_result(&status) == true);
    
    // Add elementos
    int values[] = {100, 200, 300, 400, 500, 600};
    for (int i = 0; i < 6; i++) {
        status = sat_array_add(array, &values[i]);
        assert(sat_status_get_result(&status) == true);
    }
    
    // Create múltiplos iteradores
    sat_iterator_base_t *base = (sat_iterator_base_t *)array;
    sat_iterator_t iterator1;
    sat_iterator_t iterator2;
    
    status = sat_iterator_open(&iterator1, base);
    assert(sat_status_get_result(&status) == true);

    status = sat_iterator_open(&iterator2, base);
    assert(sat_status_get_result(&status) == true);
    
    // Iterar com o primeiro iterador (apenas primeiros 3 elementos)

    int *value1;

    for (int i = 0; i < 3 && (value1 = (int *)sat_iterator_next(&iterator1)) != NULL; i++) {
        assert(value1 != NULL);
        assert(*value1 == values[i]);
    }
    
    // Iterar com o segundo iterador (all elementos)
    int count = 0;
    int *value = (int *)sat_iterator_next(&iterator2);
    while (value != NULL) {

        assert(value != NULL);
        assert(*value == values[count]);
        count++;
        value = (int *)sat_iterator_next(&iterator2);
    }
    assert(count == 6);
    
    // Continuar com o primeiro iterador (elementos restantes)
    int remaining_count = 3;
    value = (int *)sat_iterator_next(&iterator1);

    while (value != NULL) {
        
        assert(value != NULL);
        assert(*value == values[remaining_count]);
        remaining_count++;
        value = (int *)sat_iterator_next(&iterator1);
    }
    assert(remaining_count == 6);
    
    sat_array_destroy(array);
}

void test_array_iterator_dynamic_array() {
    
    sat_array_t *array = NULL;
    sat_array_args_t args = {
        .size = 2,  // Pequeno para forçar crescimento
        .object_size = sizeof(int),
        .mode = sat_array_mode_dynamic
    };
    
    sat_status_t status = sat_array_create(&array, &args);
    assert(sat_status_get_result(&status) == true);
    
    // Add elementos para forçar crescimento do array
    const int NUM_ELEMENTS = 7;
    for (int i = 0; i < NUM_ELEMENTS; i++) {
        int value = (i + 1) * 10;
        status = sat_array_add(array, &value);
        assert(sat_status_get_result(&status) == true);
    }
    
    // Verify que o array cresceu
    uint32_t capacity = 0;
    status = sat_array_get_capacity(array, &capacity);
    assert(sat_status_get_result(&status) == true);
    assert(capacity > 2);
    
    sat_iterator_base_t *base = (sat_iterator_base_t *)array;
    sat_iterator_t iterator;
    status = sat_iterator_open(&iterator, base);
    assert(sat_status_get_result(&status) == true);
    
    int count = 0;

    int *value = (int *)sat_iterator_next(&iterator);

    while (value != NULL) {
        
        assert(value != NULL);
        
        int expected_value = (count + 1) * 10;
        assert(*value == expected_value);
        count++;
        value = (int *)sat_iterator_next(&iterator);
    }
    
    assert(count == NUM_ELEMENTS);
    
    sat_array_destroy(array);
}

int main(int argc, char *argv[]) {
    
    test_array_iterator_basic();
    test_array_iterator_empty_array();
    test_array_iterator_single_element();
    test_array_iterator_modification_during_iteration();
    test_array_iterator_multiple_iterators();
    test_array_iterator_dynamic_array();
    
    return 0;
}
