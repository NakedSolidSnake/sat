#include <sat_queue_ipc.h>
#include <string.h>
#include <sys/msg.h>

#define SAT_QUEUE_DATA_BUFFER_SIZE      8192

typedef struct 
{
    long int type;
    char buffer [SAT_QUEUE_DATA_BUFFER_SIZE];

} sat_queue_data_internal_t;

static sat_status_t sat_queue_ipc_args_is_valid (sat_queue_ipc_args_t *args);
static sat_status_t sat_queue_ipc_data_is_valid (sat_queue_data_t *data);


sat_status_t sat_queue_ipc_open (sat_queue_ipc_t *object, sat_queue_ipc_args_t *args)
{
    sat_status_t status = sat_status_set (&status, false, "sat queue ipc open error");

    if (object != NULL)
    {
        memset (object, 0, sizeof (sat_queue_ipc_t));

        do 
        {

            status = sat_queue_ipc_args_is_valid (args);
            if (sat_status_get_result (&status) == false)
            {
                break;
            }

            object->id = msgget ((key_t)args->key, args->flags | IPC_CREAT);
            if (object->id == -1)
            {
                break;
            }

            object->initialized = true;
            object->key = args->key;

            sat_status_set (&status, true, "");

        } while (false);
    }

    return status;
}

sat_status_t sat_queue_ipc_send (sat_queue_ipc_t *object, sat_queue_data_t *data)
{
    sat_status_t status = sat_status_set (&status, false, "sat queue ipc send error");

    if (object != NULL && object->initialized == true)
    {
        do
        {
            status = sat_queue_ipc_data_is_valid (data);
            if (sat_status_get_result (&status) == false)
            {
                break;
            }

            sat_queue_data_internal_t __data = {.type = data->type};

            memset (__data.buffer, 0, SAT_QUEUE_DATA_BUFFER_SIZE);
            memcpy (__data.buffer, data->buffer, data->size);

            if (msgsnd (object->id, (void *)&__data, data->size, 0) == -1)
            {
                break;
            }

            sat_status_set (&status, true, "");

        } while (false);
    }

    return status;
}

sat_status_t sat_queue_ipc_receive (sat_queue_ipc_t *object, sat_queue_data_t *data)
{
    sat_status_t status = sat_status_set (&status, false, "sat queue ipc receive error");

    if (object != NULL && object->initialized == true)
    {
        do
        {
            status = sat_queue_ipc_data_is_valid (data);
            if (sat_status_get_result (&status) == false)
            {
                break;
            }

            sat_queue_data_internal_t __data = {.type = data->type};

            memset (__data.buffer, 0, SAT_QUEUE_DATA_BUFFER_SIZE);

            if (msgrcv (object->id, (void *)&__data, data->size, data->type, 0) == -1)
            {
                break;
            }

            memcpy (data->buffer, __data.buffer, data->size);

            sat_status_set (&status, true, "");

        } while (false);

        sat_status_set (&status, true, "");
    }

    return status;
}

sat_status_t sat_queue_ipc_destroy (sat_queue_ipc_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat queue ipc destroy error");

    if (object != NULL && object->initialized == true)
    {
        if (msgctl (object->id, IPC_RMID, 0) == 0)
        {
            sat_status_set (&status, true, "");
        }
    }

    return status;
}

static sat_status_t sat_queue_ipc_args_is_valid (sat_queue_ipc_args_t *args)
{
    sat_status_t status = sat_status_set (&status, false, "sat queue ipc args is valid error");

    if (args != NULL &&
        args->key > 0)
    {
        sat_status_set (&status, true, "");
    }

    return status;
}

static sat_status_t sat_queue_ipc_data_is_valid (sat_queue_data_t *data)
{
    sat_status_t status = sat_status_set (&status, false, "sat queue ipc data is valid error");

    if (data != NULL &&
        data->buffer != 0 && 
        data->size > 0 &&
        data->size < SAT_QUEUE_DATA_BUFFER_SIZE)
    {
        sat_status_set (&status, true, "");
    }

    return status;
}