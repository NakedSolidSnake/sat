#include <sat.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

#define NAME_SIZE       512

typedef struct 
{
    char name [NAME_SIZE];
    int age;
} person_t;

typedef struct 
{
    person_t person;
    double balance;
    char bank [NAME_SIZE];
} account_t;

int main (int argc, char *argv [])
{
    sat_properties_t properties;

    account_t account;

    if (argc != 2)
    {
        printf ("Usage: %s <configuration file path>\n", argv [0]);
        return -1;
    }

    memset (&account, 0, sizeof (account_t));

    // snprintf (filename, 511, "%s/%s", argv [1], "configuration.ini");

    sat_properties_map_t map = 
    {
        .items = 
        {
            {.group = "person", .field = "name",    .type = sat_properties_type_string,  .size = 512,             .data = account.person.name},
            {.group = "person", .field = "age",     .type = sat_properties_type_integer, .size = sizeof (int),    .data = &account.person.age},
            {.group = "bank",   .field = "balance", .type = sat_properties_type_decimal, .size = sizeof (double), .data = &account.balance},
            {.group = "bank",   .field = "name",    .type = sat_properties_type_string,  .size = 512,             .data = &account.bank},
        },
        .amount = 4
    };

    sat_status_t status = sat_properties_open (&properties, argv [1]);
    assert (sat_status_get_result (&status) == true);

    status = sat_properties_read (&properties, &map);
    assert (sat_status_get_result (&status) == true);

    assert (strcmp ("John Doe", account.person.name) == 0);
    assert (account.person.age == 36);
    assert (strcmp ("Some Bank SA", account.bank) == 0);
    assert (account.balance > (16.58 - 0.0001));

    return 0;
}