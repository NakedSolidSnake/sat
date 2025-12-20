#include <sat.h>
#include <assert.h>
#include <stdio.h>

bool compare (void *key, void *data)
{
    int map_key = *(int *)key;
    int user_key = *(int *)data;

    return map_key == user_key;
}

void print (const void *key, const void *value)
{
    printf ("Key: %d, Value: %d\n", *(int *)key, *(int *)value);
}

int main (int argc, char *argv[])
{
    sat_map_t *map;

    uint32_t size;

    sat_status_t status = sat_map_create (&map, &(sat_map_args_t)
                                                {
                                                    .list_size = 2,
                                                    .key_size = sizeof (int),
                                                    .value_size = sizeof (int),
                                                    .mode = sat_map_mode_static
                                                });
    assert (sat_status_get_result (&status) == true);

    status = sat_map_add (map, &(int){1}, &(int){10});
    assert (sat_status_get_result (&status) == true);

    status = sat_map_add (map, &(int){2}, &(int){5});
    assert (sat_status_get_result (&status) == true);

    status =sat_map_get_size (map, &size);
    assert (sat_status_get_result (&status) == true);
    assert (size == 2);

    sat_map_debug (map, print);

    int key = 2;
    int value = 0;

    status = sat_map_get_value_by (map, &key, &value, compare);
    assert (sat_status_get_result (&status) == true);

    key = 1;

    status = sat_map_remove (map, &key, compare);
    assert (sat_status_get_result (&status) == true);

    status = sat_map_get_size (map, &size);
    assert (sat_status_get_result (&status) == true);
    assert (size == 1);

    sat_map_debug (map, print);

    status = sat_map_destroy (map);
    assert (sat_status_get_result (&status) == true);

    return 0;
}