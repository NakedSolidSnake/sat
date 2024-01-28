#include <sat_mqtt.h>
#include <string.h>
#include <unistd.h>
#include <MQTTClient.h>

#define SAT_MQTT_TIMEOUT_DISCONNECT     10000

static sat_status_t sat_mqtt_is_args_valid (sat_mqtt_args_t *args);
static sat_status_t sat_mqtt_client_create (sat_mqtt_t *object, sat_mqtt_args_t *args);
static void sat_mqtt_args_copy_to (sat_mqtt_t *object, sat_mqtt_args_t *args);
static void sat_mqtt_callbacks_register (sat_mqtt_t *object);
static sat_status_t sat_mqtt_client_connect (sat_mqtt_t *object);
static sat_status_t sat_mqtt_is_message_valid (sat_mqtt_message_t *message);
static MQTTClient_message sat_mqtt_create_message (sat_mqtt_message_t *message);

static void sat_mqtt_on_lost_connection (void *context, char *cause);
static int sat_mqtt_on_receive (void *context, char *topic, int size, MQTTClient_message *message);



sat_status_t sat_mqtt_init (sat_mqtt_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat mqtt init error");

    if (object != NULL)
    {
        memset (object, 0, sizeof (sat_mqtt_t));

        sat_status_set (&status, true, "");
    }

    return status;
}

sat_status_t sat_mqtt_open (sat_mqtt_t *object, sat_mqtt_args_t *args)
{
    sat_status_t status = sat_status_set (&status, false, "sat mqtt open error");

    if (object != NULL && args != NULL)
    {
        
        do
        {
            status = sat_mqtt_is_args_valid (args);
            if (sat_status_get_result (&status) == false)
                break;

            status = sat_mqtt_client_create (object, args);
            if (sat_status_get_result (&status) == false)
                break;

            sat_mqtt_args_copy_to (object, args);

            sat_mqtt_callbacks_register (object);

            status = sat_mqtt_client_connect (object);

        } while (false);
    }

    return status;
}

sat_status_t sat_mqtt_publish (sat_mqtt_t *object, sat_mqtt_message_t *message)
{
    sat_status_t status = sat_status_set (&status, false, "sat mqtt publish error");

    if (object != NULL && object->type == sat_mqtt_type_publisher)
    {
        status = sat_mqtt_is_message_valid (message);
        if (sat_status_get_result (&status) == true)
        {
            MQTTClient_message __message = sat_mqtt_create_message (message);

            if (MQTTClient_publishMessage (object->handle,
                                           message->topic,
                                           &__message,
                                           NULL) == MQTTCLIENT_SUCCESS)
            {
                sat_status_set (&status, true, "");
            }
        }
    }

    return status;
}

sat_status_t sat_mqtt_subscribe (sat_mqtt_t *object, char *topic, sat_mqtt_qos_type_t qos)
{
    sat_status_t status = sat_status_set (&status, false, "sat mqtt subscribe error");

    if (object != NULL && object->handle != NULL && topic != NULL && object->type == sat_mqtt_type_subscriber)
    {
        MQTTClient_subscribe (object->handle, topic, qos);

        while (true)
        {
            usleep (1);
        }

        sat_status_set (&status, true, "");
    }

    return status;
}

sat_status_t sat_mqtt_close (sat_mqtt_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat mqtt close error");

    if (object != NULL && object->handle != NULL)
    {
        MQTTClient_disconnect (object->handle, SAT_MQTT_TIMEOUT_DISCONNECT);
        MQTTClient_destroy (&object->handle);

        memset (object, 0, sizeof (sat_mqtt_t));
        sat_status_set (&status, true, "");
    }

    return status;
}

static sat_status_t sat_mqtt_is_args_valid (sat_mqtt_args_t *args)
{
    sat_status_t status = sat_status_set (&status, false, "sat mqtt is args valid error");

    if (args->address != NULL && 
        args->id != NULL)
    {
        sat_status_set (&status, true, "");
    }

    return status;
}

static sat_status_t sat_mqtt_client_create (sat_mqtt_t *object, sat_mqtt_args_t *args)
{
    sat_status_t status = sat_status_set (&status, false, "sat mqtt client create error");

    if (MQTTClient_create (&object->handle,
                           args->address,
                           args->id,
                           args->persistence,
                           NULL) == MQTTCLIENT_SUCCESS)
    {
        sat_status_set (&status, true, "");
    }

    return status;
}

static void sat_mqtt_args_copy_to (sat_mqtt_t *object, sat_mqtt_args_t *args)
{
    object->type = args->type;
    object->data = args->data;

    object->events.on_receive = args->events.on_receive;
    object->events.on_lost_connection = args->events.on_lost_connection;
}

static void sat_mqtt_callbacks_register (sat_mqtt_t *object)
{
    MQTTClient_setCallbacks (object->handle,
                             (void *)object,
                             sat_mqtt_on_lost_connection,
                             sat_mqtt_on_receive,
                             NULL);
}

static sat_status_t sat_mqtt_client_connect (sat_mqtt_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat mqtt client connect error");

    MQTTClient_connectOptions options = MQTTClient_connectOptions_initializer;

    if (MQTTClient_connect (object->handle, &options) == MQTTCLIENT_SUCCESS)
    {
        sat_status_set (&status, true, "");
    }

    return status;
}

static sat_status_t sat_mqtt_is_message_valid (sat_mqtt_message_t *message)
{
    sat_status_t status = sat_status_set (&status, false, "sat mqtt is message valid error");


    if (message->payload != NULL &&
        message->size > 0 &&
        message->topic != NULL &&
        strlen (message->topic) > 0)
    {
        sat_status_set (&status, true, "");
    }

    return status;
}

static MQTTClient_message sat_mqtt_create_message (sat_mqtt_message_t *message)
{
    MQTTClient_message __message = MQTTClient_message_initializer;

    __message.payload = message->payload;
    __message.payloadlen = message->size;
    __message.qos = message->qos;
    __message.retained = 0;

    return __message;
}

static void sat_mqtt_on_lost_connection (void *context, char *cause)
{
    sat_mqtt_t *object = (sat_mqtt_t *)context;

    if (object->events.on_lost_connection != NULL)
    {
        object->events.on_lost_connection (object->data, cause);
    }
}

static int sat_mqtt_on_receive (void *context, char *topic, int size, MQTTClient_message *message)
{
    sat_mqtt_t *object = (sat_mqtt_t *)context;
    sat_mqtt_message_t __message;

    memset (&__message, 0, sizeof (sat_mqtt_message_t));

    __message.topic = topic;
    __message.payload = message->payload;
    __message.size = message->payloadlen;

    if (object->events.on_receive != NULL)
    {
        object->events.on_receive (object->data, __message);
    }

    MQTTClient_freeMessage (&message);
    MQTTClient_free (topic);

    return 1;
}