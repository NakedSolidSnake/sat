#include <sat.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

/**
 * @brief Tests for advanced sat_array operations
 */

typedef struct
{
    int id;
    char name [32];
    float score;
    bool active;
} student_t;

// Comparison function for search by ID
bool compare_by_id (void *element, void *param)
{
    student_t *student = (student_t *)element;
    int *search_id = (int *)param;
    return student->id == *search_id;
}

// Comparison function for search by name
bool compare_by_name (void *element, void *param)
{
    student_t *student = (student_t *)element;
    char *search_name = (char *)param;
    return strcmp (student->name, search_name) == 0;
}

// Comparison function for search by active status
bool compare_by_active_status (void *element, void *param)
{
    student_t *student = (student_t *)element;
    bool *search_active = (bool *)param;
    return student->active == *search_active;
}

void test_array_search_by_parameter (void)
{
    sat_array_t *array = NULL;
    sat_array_args_t args =
    {
        .size = 10,
        .object_size = sizeof (student_t),
        .mode = sat_array_mode_static
    };
    
    sat_status_t status = sat_array_create (&array, &args);
    assert (sat_status_get_result (&status) == true);

    // Add students de teste
    student_t students [] =
    {
        { 1001, "Alice"  , 85.5f, true  },
        { 1002, "Bob"    , 92.0f, true  },
        { 1003, "Charlie", 78.3f, false },
        { 1004, "Diana"  , 96.7f, true  },
        { 1005, "Eve"    , 82.1f, false }
    };
    
    for (int i = 0; i < 5; i++)
    {
        status = sat_array_add (array, &students [i]);
        assert (sat_status_get_result (&status) == true);
    }
    
    // Test: Buscar por ID
    int search_id = 1003;
    student_t found_student;
    status = sat_array_get_object_by_parameter (array, &search_id, compare_by_id, &found_student);
    assert (sat_status_get_result (&status) == true);
    assert (found_student.id == 1003);
    assert (strcmp (found_student.name, "Charlie") == 0);

    // Test: Buscar por nome
    char search_name [] = "Diana";
    status = sat_array_get_object_by_parameter (array, search_name, compare_by_name, &found_student);
    assert (sat_status_get_result (&status) == true);
    assert (found_student.id == 1004);
    assert (strcmp (found_student.name, "Diana") == 0);

    // Test: Buscar por status ativo
    bool search_active = true;
    status = sat_array_get_object_by_parameter (array, &search_active, compare_by_active_status, &found_student);
    assert (sat_status_get_result (&status) == true);
    assert (found_student.active == true);
    
    // Test: Buscar elemento que não existe
    int nonexistent_id = 9999;
    status = sat_array_get_object_by_parameter (array, &nonexistent_id, compare_by_id, &found_student);
    assert (sat_status_get_result (&status) == false);

    sat_array_destroy (array);
}

void test_array_reference_access (void)
{
    sat_array_t *array = NULL;
    sat_array_args_t args =
    {
        .size = 5,
        .object_size = sizeof (student_t),
        .mode = sat_array_mode_static
    };

    sat_status_t status = sat_array_create (&array, &args);
    assert (sat_status_get_result (&status) == true);
    
    // Add some students
    student_t students [] =
    {
        {2001, "John", 88.0f, true  },
        {2002, "Jane", 94.5f, true  },
        {2003, "Jack", 76.2f, false }
    };
    
    for (int i = 0; i < 3; i++)
    {
        status = sat_array_add (array, &students [i]);
        assert (sat_status_get_result (&status) == true);
    }
    
    // Get reference direta para elementos
    student_t *ref1 = (student_t *)sat_array_get_reference_by (array, 0);
    assert (ref1 != NULL);
    assert (ref1->id == 2001);
    assert (strcmp (ref1->name, "John") == 0);

    student_t *ref2 = (student_t *)sat_array_get_reference_by (array, 1);
    assert (ref2 != NULL);
    assert (ref2->id == 2002);

    // Modify elements through referência
    strcpy (ref1->name, "Modified John");
    ref1->score = 99.9f;
    
    // Verify que a modificação foi aplicada
    student_t retrieved_student;
    status = sat_array_get_object_by (array, 0, &retrieved_student);
    assert (sat_status_get_result (&status) == true);
    assert (strcmp (retrieved_student.name, "Modified John") == 0);
    assert (retrieved_student.score == 99.9f);

    // Test com index inválido
    student_t *invalid_ref = (student_t *)sat_array_get_reference_by (array, 10);
    assert (invalid_ref == NULL);

    sat_array_destroy (array);
}

void test_array_operations_with_null_parameters (void)
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

    int value = 42;
    status = sat_array_add (array, &value);
    assert (sat_status_get_result (&status) == true);
    
    // Test get_object_by_parameter with parameters NULL
    int result;
    
    // Compare function NULL
    status = sat_array_get_object_by_parameter (array, &value, NULL, &result);
    assert (sat_status_get_result (&status) == false);

    // Parameter NULL
    status = sat_array_get_object_by_parameter (array, NULL, compare_by_id, &result);
    assert (sat_status_get_result (&status) == false);
    
    // Output data NULL
    status = sat_array_get_object_by_parameter (array, &value, compare_by_id, NULL);
    assert (sat_status_get_result (&status) == false);

    // Test get_size com NULL
    status = sat_array_get_size (array, NULL);
    assert (sat_status_get_result (&status) == false);

    // Test get_capacity com NULL
    status = sat_array_get_capacity (array, NULL);
    assert (sat_status_get_result (&status) == false);

    sat_array_destroy (array);
}

void test_array_complex_data_operations (void)
{
    sat_array_t *array = NULL;
    sat_array_args_t args =
    {
        .size = 8,
        .object_size = sizeof (student_t),
        .mode = sat_array_mode_dynamic
    };

    sat_status_t status = sat_array_create (&array, &args);
    assert (sat_status_get_result (&status) == true);

    // Create um conjunto mais complexo de dados
    student_t students [] =
    {
        {3001, "Alpha"  , 95.0f, true  },
        {3002, "Beta"   , 87.5f, false },
        {3003, "Gamma"  , 92.3f, true  },
        {3004, "Delta"  , 78.8f, false },
        {3005, "Epsilon", 89.1f, true  },
        {3006, "Zeta"   , 94.7f, true  },
        {3007, "Eta"    , 83.2f, false },
        {3008, "Theta"  , 91.5f, true  }
    };
    
    // Add all students
    for (int i = 0; i < 8; i++)
    {
        status = sat_array_add (array, &students [i]);
        assert (sat_status_get_result (&status) == true);
    }
    
    // Complex operation 1: Find all students ativos
    int active_count = 0;
    for (uint32_t i = 0; i < 8; i++)
    {
        student_t *ref = (student_t *)sat_array_get_reference_by (array, i);
        if (ref && ref->active)
        {
            active_count++;
        }
    }
    
    // Complex operation 2: Update scores of students inativos
    for (uint32_t i = 0; i < 8; i++)
    {
        student_t *ref = (student_t *)sat_array_get_reference_by (array, i);
        if (ref && !ref->active)
        {
            float old_score = ref->score;
            ref->score += 5.0f;  // Bonus para inativos
        }
    }
    
    // Complex operation 3: Remover estudante com menor score
    float min_score = 1000.0f;
    uint32_t min_index = 0;
    for (uint32_t i = 0; i < 8; i++)
    {
        student_t *ref = (student_t *)sat_array_get_reference_by (array, i);
        if (ref && ref->score < min_score)
        {
            min_score = ref->score;
            min_index = i;
        }
    }
    
    student_t removed_student;
    status = sat_array_get_object_by (array, min_index, &removed_student);
    assert (sat_status_get_result (&status) == true);

    status = sat_array_remove_by (array, min_index);
    assert (sat_status_get_result (&status) == true);
    
    // Verify size final
    uint32_t final_size;
    status = sat_array_get_size (array, &final_size);
    assert (sat_status_get_result (&status) == true);
    assert (final_size == 7);

    sat_array_destroy (array);
}

int main (int argc, char *argv []) 
{
    test_array_search_by_parameter ();
    test_array_reference_access ();
    test_array_operations_with_null_parameters ();
    test_array_complex_data_operations ();
    
    return 0;
}
