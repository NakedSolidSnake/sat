#include <sat_kafka.h>
#include <sat_kafka_configuration.h>
#include <string.h>
#include <glib-2.0/glib.h>

static sat_status_t sat_kafka_instance_create (sat_kafka_t *object, sat_kafka_configuration_t *configuration);
static sat_status_t sat_kafka_configure_by_type (sat_kafka_t *object, sat_kafka_configuration_t *configuration, sat_kafka_args_t *args);
static enum rd_kafka_type_t sat_kafka_get_type (sat_kafka_t *object);
static bool sat_kafka_topics_subscription (sat_kafka_t *object, sat_kafka_topics_t *topics);
static bool sat_kafka_set_records_handler (sat_kafka_t *object, sat_kafka_args_t *args);

bool sat_kafka_is_message_error (sat_kafka_t *object, rd_kafka_message_t *message);
void sat_kafka_process_message (sat_kafka_t *object, rd_kafka_message_t *message);

static void sat_kafka_publisher_on_error (rd_kafka_t *handle, const rd_kafka_message_t *rkmessage, void *opaque);

sat_status_t sat_kafka_init (sat_kafka_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat kafka init error");
    
    if (object != NULL)
    {
        memset (object, 0, sizeof (sat_kafka_t));

        sat_status_set (&status, true, "");
    }

    return status;
}

sat_status_t sat_kafka_open (sat_kafka_t *object, sat_kafka_args_t *args)
{
    sat_status_t status = sat_status_set (&status, false, "sat kafka open error");
    
    if (object != NULL && args != NULL)
    {
        sat_kafka_configuration_t configuration;

        object->type = args->type;

        do 
        {
            status = sat_kafka_configuration_load (&configuration, args->file, &args->groups);
            sat_status_break_on_error (status);

            status = sat_kafka_instance_create (object, &configuration);
            sat_status_break_on_error (status);

            status = sat_kafka_configure_by_type (object, &configuration, args);
            sat_status_break_on_error (status);

            status = sat_kafka_configuration_unload (&configuration);

        } while (false);
    }

    return status;
}

sat_status_t sat_kafka_send (sat_kafka_t *object, const char *topic, sat_kafka_message_t *message, int timeout)
{
    sat_status_t status = sat_status_set (&status, false, "sat kafka send error");
    
    if (object != NULL && object->type == sat_kafka_type_publisher && topic != NULL && message != NULL)
    {
        rd_kafka_resp_err_t error;

        error = rd_kafka_producev (object->instance,
                                   RD_KAFKA_V_TOPIC (topic),
                                   RD_KAFKA_V_MSGFLAGS (RD_KAFKA_MSG_F_COPY),
                                   RD_KAFKA_V_KEY ((void *)message->key.data, message->key.size),
                                   RD_KAFKA_V_VALUE ((void *)message->payload.data, message->payload.size),
                                   RD_KAFKA_V_OPAQUE (NULL),
                                   RD_KAFKA_V_END);

        if (error == RD_KAFKA_RESP_ERR_NO_ERROR)
        {
            rd_kafka_poll (object->instance, 0);
            rd_kafka_flush (object->instance, timeout);

            sat_status_set (&status, true, "");
        }
    }

    return status;
}

sat_status_t sat_kafka_poll (sat_kafka_t *object, int timeout)
{
    sat_status_t status = sat_status_set (&status, false, "sat kafka poll error");

    rd_kafka_message_t *message;
    
    if (object != NULL && object->type == sat_kafka_type_subscriber)
    {
        message = rd_kafka_consumer_poll (object->instance, timeout);

        if (message != NULL)
        {
            if (sat_kafka_is_message_error (object, message) == false)
            {
                sat_kafka_process_message (object, message);
                sat_status_set (&status, true, "");
            }

            rd_kafka_message_destroy (message);
        }
    }

    return status;
}

sat_status_t sat_kafka_close (sat_kafka_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat kafka close error");
    
    if (object != NULL)
    {
        if (object->type == sat_kafka_type_subscriber)
            rd_kafka_consumer_close (object->instance);

        rd_kafka_destroy (object->instance);

        memset (object, 0, sizeof (sat_kafka_t));

        sat_status_set (&status, true, "");
    }

    return status;
}

static sat_status_t sat_kafka_instance_create (sat_kafka_t *object, sat_kafka_configuration_t *configuration)
{
    sat_status_t status = sat_status_set (&status, false, "sat kafka instance create error");

    enum rd_kafka_type_t type = sat_kafka_get_type (object);

    object->instance = rd_kafka_new (type, configuration->configuration, object->error, sizeof (object->error));

    if (object->instance != NULL)
        sat_status_set (&status, true, "");

    return status;
}

static sat_status_t sat_kafka_configure_by_type (sat_kafka_t *object, sat_kafka_configuration_t *configuration, sat_kafka_args_t *args)
{
    sat_status_t status = sat_status_set (&status, false, "sat kafka configure by type error");

    switch (object->type)
    {
    case sat_kafka_type_publisher:
        rd_kafka_conf_set_dr_msg_cb (configuration->configuration, sat_kafka_publisher_on_error);
        sat_status_set (&status, true, "");
        break;

    case sat_kafka_type_subscriber:
        sat_kafka_topics_subscription (object, &args->topics);
        sat_kafka_set_records_handler (object, args);
        sat_status_set (&status, true, "");
        break;
    
    default:
        break;
    }

    return status;
}

static enum rd_kafka_type_t sat_kafka_get_type (sat_kafka_t *object)
{
    enum rd_kafka_type_t type = RD_KAFKA_PRODUCER;

    if (object->type == sat_kafka_type_subscriber)
        type = RD_KAFKA_CONSUMER;

    return type;
}

static bool sat_kafka_topics_subscription (sat_kafka_t *object, sat_kafka_topics_t *topics)
{
    bool status = false;

    rd_kafka_topic_partition_list_t *subscriptions = rd_kafka_topic_partition_list_new (topics->amount);

    for (uint8_t i = 0; i < topics->amount; i++)
    {
        rd_kafka_topic_partition_list_add (subscriptions, topics->list[i], RD_KAFKA_PARTITION_UA);
    }

    if (rd_kafka_subscribe (object->instance, subscriptions) == RD_KAFKA_RESP_ERR_NO_ERROR)
        status = true;

    rd_kafka_topic_partition_list_destroy (subscriptions);

    return status;
}

static bool sat_kafka_set_records_handler (sat_kafka_t *object, sat_kafka_args_t *args)
{
    bool status = false;

    if (args->base != NULL)
    {
        object->base = args->base;
        status = true;
    }

    return status;
}

static void sat_kafka_publisher_on_error (rd_kafka_t *handle, const rd_kafka_message_t *rkmessage, void *opaque)
{
    (void)handle;
    (void)opaque;

    if (rkmessage->err)
    {
        g_error ("Message delivery failed: %s", rd_kafka_err2str (rkmessage->err));
    }
}

bool sat_kafka_is_message_error (sat_kafka_t *object, rd_kafka_message_t *message)
{
    (void) object;

    bool status = false;

    if (message->err)
    {
        if (message->err != RD_KAFKA_RESP_ERR__PARTITION_EOF)
            status = true;
    }

    return status;
}

void sat_kafka_process_message (sat_kafka_t *object, rd_kafka_message_t *message)
{
    if (object->base->on_record != NULL)
    {
        sat_kafka_message_t _message = 
        {
            .topic = rd_kafka_topic_name (message->rkt),
            .key = 
            {
                .data = message->key,
                .size = message->key_len
            },
            .payload = 
            {
                .data = message->payload,
                .size = message->len
            }
        };

        object->base->on_record (object->base->object, &_message);
    }
}