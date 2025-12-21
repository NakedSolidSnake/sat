#include <stdio.h>
#include <sat_json.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct 
{
    char name [512];
    int age;
} person_t;

typedef struct 
{
    person_t parent;
    person_t children [3];
} registry_t;

typedef struct 
{
    char street [128];
    int number;
    char zipcode [10];
} address_t;

typedef struct 
{
    person_t person;
    address_t address;
} owner_t;

typedef struct 
{
    int data [10];    
} values_t;

sat_json_mapper_t *sat_json_get_next_person (void *object, uint16_t index)
{
    person_t *persons = (person_t *)object;

    sat_json_mapper_t child_mapper [] = 
    {
        {.token = "name", .data = persons [index].name, .type = sat_json_type_string, .size = 512},
        {.token = "age",  .data = &persons [index].age, .type = sat_json_type_int,    .size = sizeof (int)}
    };

    sat_json_mapper_t *mapper = (sat_json_mapper_t *) calloc (1, sizeof (child_mapper));

    memcpy (mapper, child_mapper, sizeof (child_mapper));

    return mapper;
}

sat_json_mapper_t *sat_json_get_next_value (void *object, uint16_t index)
{
    values_t *values = (values_t *)object;

    sat_json_mapper_t value_mapper [] = 
    {
        {.token = NULL,  .data = &values->data [index], .type = sat_json_type_int, .size = sizeof (int)}
    };

    sat_json_mapper_t *mapper = (sat_json_mapper_t *) calloc (1, sizeof (value_mapper));

    memcpy (mapper, value_mapper, sizeof (value_mapper));

    return mapper;
}

void sat_json_print_registry (registry_t *object);
void sat_json_print_owner (owner_t *object);
void sat_json_print_values (values_t *object);

bool sat_json_test_serialize_object (void);
bool sat_json_test_serialize_object_object (void);
bool sat_json_test_serialize_array (void);
bool sat_json_test_serialize_array2 (void);

bool sat_json_test_deserialize_object_array (void);
bool sat_json_test_deserialize_array (void);
bool sat_json_test_deserialize_array_primitives (void);
bool sat_json_test_deserialize_array_primitives_no_token (void);
bool sat_json_test_deserialize_object_object (void);

int main (int argc, char *argv[])
{
    sat_json_test_serialize_object ();
    sat_json_test_serialize_object_object ();
    sat_json_test_serialize_array ();
    sat_json_test_serialize_array2 ();
    sat_json_test_deserialize_object_array ();
    sat_json_test_deserialize_array ();
    sat_json_test_deserialize_array_primitives ();
    sat_json_test_deserialize_array_primitives_no_token ();
    sat_json_test_deserialize_object_object ();

    return 0;
}


void sat_json_print_registry (registry_t *object)
{
    printf ("Parent\n"
            "Name: %s\n"
            "Age: %d\n", object->parent.name, object->parent.age);
    
    printf ("\nChildren:\n");

    for (uint8_t i = 0; i < (sizeof (object->children) / sizeof (object->children [0])); i++)
    {
        person_t *p = &object->children [i];

        printf ("Name: %s\n"
                "Age: %d\n", p->name, p->age);
    }
}

void sat_json_print_owner (owner_t *object)
{
    printf ("Owner\n"
            "Name: %s\n"
            "Age: %d\n", object->person.name, object->person.age);  
            
    printf ("Address\n"
            "Street: %s Number: %d Zipcode: %s\n", object->address.street,
                                                   object->address.number,
                                                   object->address.zipcode);
}

void sat_json_print_values (values_t *object)
{
    for (int i = 0; i < sizeof (object->data) / sizeof (object->data [0]); i++)
    {
        printf ("%d\n", object->data [i]);
    }
}

bool sat_json_test_serialize_object (void)
{
    sat_json_t root;
    sat_json_t array;
    sat_json_t object;

    char buffer_root [1024] = {0};
    char buffer_array [1024] = {0};
    char buffer_object [1024] = {0};

    char *s = NULL;

    person_t person = 
    {
        .name = "John Doe",
        .age = 36
    };

    sat_json_init (&root);
    sat_json_init (&array);
    sat_json_init (&object);

    sat_json_open (&root, &(sat_json_args_t){.buffer = buffer_root, .size = 1024});
    sat_json_open (&array, &(sat_json_args_t){.buffer = buffer_array, .size = 1024});
    sat_json_open (&object, &(sat_json_args_t){.buffer = buffer_object, .size = 1024});

    sat_json_serialize_create_object (&root);
    sat_json_serialize_create_object (&object);
    sat_json_serialize_create_array (&array);

    sat_json_serialize_add (&object, sat_json_type_string, "name", person.name);
    sat_json_serialize_add (&object, sat_json_type_int, "age", &person.age);

    sat_json_serialize_add (&array, sat_json_type_object, "", object.json);

    sat_json_serialize_add (&root, sat_json_type_string, "name", person.name);
    sat_json_serialize_add (&root, sat_json_type_int, "age", &person.age);

    sat_json_serialize_add (&root, sat_json_type_array, "persons", array.json);

    sat_json_to_string (&root, &s);

    printf ("%s\n", s);
    
    sat_json_close (&root);

    return false;
}

bool sat_json_test_serialize_object_object (void)
{
    sat_json_t root;
    sat_json_t object;

    char buffer_root [1024] = {0};
    char buffer_object [1024] = {0};

    char buffer_user [1024] = {0};

    char *s = NULL;

    person_t person = 
    {
        .name = "John Doe",
        .age = 36
    };

    address_t address = 
    {
        .street = "New Street",
        .number = 1234,
        .zipcode = "12345678"
    };

    sat_json_init (&root);
    sat_json_init (&object);

    sat_json_open (&root, &(sat_json_args_t){.buffer = buffer_root, .size = 1024});
    sat_json_open (&object, &(sat_json_args_t){.buffer = buffer_object, .size = 1024});

    sat_json_serialize_create_object (&root);
    sat_json_serialize_create_object (&object);

    sat_json_serialize_add (&object, sat_json_type_string, "street", address.street);
    sat_json_serialize_add (&object, sat_json_type_int, "number", &address.number);
    sat_json_serialize_add (&object, sat_json_type_string, "zipcode", address.zipcode);

    sat_json_serialize_add (&root, sat_json_type_string, "name", person.name);
    sat_json_serialize_add (&root, sat_json_type_int, "age", &person.age);

    sat_json_serialize_add (&root, sat_json_type_object, "address", object.json);

    sat_json_to_string (&root, &s);

    printf ("%s\n", s);

    sat_json_string_to_buffer (&root, buffer_user, sizeof (buffer_user));

    printf ("%s\n", buffer_user);
    
    sat_json_close (&root);

    return false;
}

bool sat_json_test_serialize_array (void)
{
    sat_json_t array;
    sat_json_t object;

    char buffer_array [1024] = {0};
    char buffer_object [1024] = {0};

    char *s = NULL;

    person_t person = 
    {
        .name = "John Doe",
        .age = 36
    };

    sat_json_init (&array);
    sat_json_init (&object);

    sat_json_open (&array, &(sat_json_args_t){.buffer = buffer_array, .size = 1024});
    sat_json_open (&object, &(sat_json_args_t){.buffer = buffer_object, .size = 1024});

    sat_json_serialize_create_object (&object);
    sat_json_serialize_create_array (&array);

    sat_json_serialize_add (&object, sat_json_type_string, "name", person.name);
    sat_json_serialize_add (&object, sat_json_type_int, "age", &person.age);

    sat_json_serialize_add (&array, sat_json_type_object, "", object.json);

    sat_json_to_string (&array, &s);

    printf ("%s\n", s);
    
    sat_json_close (&array);
    
    return false;
}

bool sat_json_test_serialize_array2 (void)
{
    sat_json_t array;

    char buffer_array [1024] = {0};

    char *s = NULL;


    sat_json_init (&array);

    sat_json_open (&array, &(sat_json_args_t){.buffer = buffer_array, .size = 1024});

    sat_json_serialize_create_array (&array);

    sat_json_serialize_add (&array, sat_json_type_string, "", "hello");

    sat_json_to_string (&array, &s);

    printf ("%s\n", s);
    
    sat_json_close (&array);
    
    return false;
}

bool sat_json_test_deserialize_object_array (void)
{
    const char *template = 
    "{"
        "\"parent\": "
        "{"
            "\"name\": \"John Doe\","
            "\"age\" : 26,"
            "\"children\" :"
            "["
                "{"
                "\"name\": \"Peter Doe\","
                "\"age\": 16"
                "},"
                "{"
                "\"name\": \"Marta Doe\","
                "\"age\": 17"
                "},"
                "{"
                "\"name\": \"Gwen Doe\","
                "\"age\": 14"
                "}"
            "]"
        "}"
    "}";

    sat_json_t json;
    sat_json_init (&json);

    registry_t registry;

    memset (&registry, 0, sizeof (registry_t));

    sat_json_mapper_t children_mapper [] = 
    {
        {.token = NULL, .data = registry.children, .get_next = sat_json_get_next_person, .type = sat_json_type_object, .fields = 2}
    };

    sat_json_mapper_t parent_mapper [] = 
    {
        {.token = "name", .data = registry.parent.name, .type = sat_json_type_string, .size = 512},
        {.token = "age", .data = &registry.parent.age, .type = sat_json_type_int, .size = sizeof (int)},
        {.token = "children", .type = sat_json_type_array, .child = children_mapper},
    };

    sat_json_mapper_t root = 
    {
        .token = "parent", .type = sat_json_type_object, .child = parent_mapper, .fields = 3
    };

    sat_json_deserialize (&json, template, &root, 1);

    sat_json_print_registry (&registry);

    return false;
}

bool sat_json_test_deserialize_array (void)
{
    char *template = 
    "["
        "{"
        "\"name\": \"Peter Doe\","
        "\"age\": 16"
        "},"
        "{"
        "\"name\": \"Marta Doe\","
        "\"age\": 17"
        "},"
        "{"
        "\"name\": \"Gwen Doe\","
        "\"age\": 14"
        "}"
    "]";

    sat_json_t json;
    sat_json_init (&json);

    registry_t registry;

    memset (&registry, 0, sizeof (registry_t));

    sat_json_mapper_t children_mapper [] = 
    {
        {.token = NULL, .data = registry.children, .get_next = sat_json_get_next_person, .type = sat_json_type_object, .fields = 2}
    };

    sat_json_mapper_t array_mapper = 
    {
        .token = "", .type = sat_json_type_array, .child = children_mapper
    };

    sat_json_deserialize (&json, template, &array_mapper, 1);

    sat_json_print_registry (&registry);

    return false;
}

bool sat_json_test_deserialize_array_primitives (void)
{
    char *template = 
    "{"
        "\"values\" :"
        "["
            "10,"
            "20,"
            "30,"
            "40,"
            "50,"
            "60,"
            "70,"
            "80,"
            "90,"
            "99"
        "]"
    "}";

    values_t values;

    memset (&values, 0, sizeof (values_t));

    sat_json_mapper_t values_mapper [] = 
    {
        {.token = NULL, .data = &values, .get_next = sat_json_get_next_value, .fields = 1}
    };

    sat_json_mapper_t array_mapper = 
    {
        .token = "values", .type = sat_json_type_array, .child = values_mapper
    };

    sat_json_t json;
    sat_json_init (&json);

    sat_json_deserialize (&json, template, &array_mapper, 1);

    sat_json_print_values (&values);
    return false;
}

bool sat_json_test_deserialize_array_primitives_no_token (void)
{
    char *template = 
    "["
        "10,"
        "20,"
        "30,"
        "40,"
        "50,"
        "60,"
        "70,"
        "80,"
        "90,"
        "99"
    "]";

    values_t values;

    memset (&values, 0, sizeof (values_t));

    sat_json_mapper_t values_mapper [] = 
    {
        {.token = NULL, .data = &values, .get_next = sat_json_get_next_value, .fields = 1}
    };

    sat_json_mapper_t array_mapper = 
    {
        .token = NULL, .type = sat_json_type_array, .child = values_mapper
    };

    sat_json_t json;
    sat_json_init (&json);

    sat_json_deserialize (&json, template, &array_mapper, 1);

    sat_json_print_values (&values);

    return false;
}

bool sat_json_test_deserialize_object_object (void)
{
    char *template = 
    "{"
        "\"name\" : \"John Doe\","
        "\"age\" : 26,"
        "\"address\" :"
        "{"
            "\"street\" : \"New Street\","
            "\"number\" : 124,"
            "\"zipcode\" : \"12345678\""
        "}"  
    "}";

    sat_json_t json;
    owner_t owner;

    memset (&owner, 0, sizeof (owner_t));

    sat_json_mapper_t address_mapper [] = 
    {
        {.token = "street", .data = owner.address.street, .type = sat_json_type_string, .size = 128},
        {.token = "number", .data = &owner.address.number, .type = sat_json_type_int, .size = sizeof (int)},
        {.token = "zipcode", .data = owner.address.zipcode, .type = sat_json_type_string, .size = 10},
    };

    sat_json_mapper_t person_mapper [] = 
    {
        {.token = "name", .data = owner.person.name, .type = sat_json_type_string, .size = 512},
        {.token = "age", .data = &owner.person.age, .type = sat_json_type_int, .size = sizeof (int)},
        {.token = "address", .type = sat_json_type_object, .child = address_mapper, .fields = 3},
    };

    sat_json_init (&json);

    sat_json_deserialize (&json, template, person_mapper, 3);

    sat_json_print_owner (&owner);

    return false;
}
