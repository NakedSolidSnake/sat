#include <sat_kafka_configuration.h>
#include <sat_kafka_common.h>
#include <glib-2.0/glib.h>

sat_status_t sat_kafka_configuration_load (sat_kafka_configuration_t *object, const char *file, sat_kafka_groups_t *groups)
{
    sat_status_t status = sat_status_set (&status, false, "sat kafka configuration load error");

    g_autoptr (GError) error = NULL;
    g_autoptr (GKeyFile) key_file = g_key_file_new ();

    if (g_key_file_load_from_file (key_file, file, G_KEY_FILE_NONE, &error) != 0)
    {
        object->configuration = rd_kafka_conf_new ();

        for (int i = 0; i < groups->amount; i++)
        {
            sat_kafka_load_group (object->configuration, key_file, groups->list[i]);
        }

        sat_status_set (&status, true, "");
    }

    return status;
}

sat_status_t sat_kafka_configuration_unload (sat_kafka_configuration_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat kafka configuration unload error");

    if (object->configuration != NULL)
    {
        object->configuration = NULL;

        sat_status_set (&status, true, "");
    }

    return status;
}