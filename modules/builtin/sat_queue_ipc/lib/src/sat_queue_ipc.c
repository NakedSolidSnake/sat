#include <sat_queue_ipc.h>
#include <string.h>
#include <sys/msg.h>

#define SAT_QUEUE_DATA_BUFFER_SIZE      8192

typedef struct 
{
    long int type;
    char buffer [SAT_QUEUE_DATA_BUFFER_SIZE];

} sat_queue_data_internal_t;

static sat_status_t sat_queue_ipc_args_is_valid (const sat_queue_ipc_args_t *const args);
static sat_status_t sat_queue_ipc_data_is_valid (const sat_queue_data_t *const data);


sat_status_t sat_queue_ipc_open (sat_queue_ipc_t *const object, const sat_queue_ipc_args_t *const args)
{
    sat_status_t status = sat_status_success (&status);

    do
    {
        if (object == NULL)
        {
            sat_status_set (&status, false, "sat queue ipc open error: null object");
            break;
        }

        status = sat_queue_ipc_args_is_valid (args);
        sat_status_break_on_error (status);

        memset (object, 0, sizeof (sat_queue_ipc_t));

        object->id = msgget ((key_t)args->key, args->flags | IPC_CREAT);
        if (object->id == -1)
        {
            break;
        }

        object->initialized = true;
        object->key = args->key;

    } while (false);

    return status;
}

sat_status_t sat_queue_ipc_send (sat_queue_ipc_t *const object, const sat_queue_data_t *const data)
{
    sat_status_t status = sat_status_set (&status, false, "sat queue ipc send error");

    do
    {
        if (object == NULL)
        {
            sat_status_set (&status, false, "sat queue ipc send error: null object");
            break;
        }

        if (object->initialized == false)
        {
            sat_status_set (&status, false, "sat queue ipc send error: not initialized");
            break;
        }

        status = sat_queue_ipc_data_is_valid (data);
        sat_status_break_on_error (status);

        sat_queue_data_internal_t __data = {.type = data->type};

        memset (__data.buffer, 0, SAT_QUEUE_DATA_BUFFER_SIZE);
        memcpy (__data.buffer, data->buffer, data->size);

        if (msgsnd (object->id, (void *)&__data, data->size, 0) == -1)
        {
            sat_status_set (&status, false, "sat queue ipc send error: msgsnd failed");
        }
            
    } while (false);

    return status;
}

sat_status_t sat_queue_ipc_receive (sat_queue_ipc_t *const object, sat_queue_data_t *const data)
{
    sat_status_t status = sat_status_success (&status);

    do
    {
        if (object == NULL)
        {
            sat_status_set (&status, false, "sat queue ipc receive error: null object");
            break;
        }

        if (object->initialized == false)
        {
            sat_status_set (&status, false, "sat queue ipc receive error: not initialized");
            break;
        }

        status = sat_queue_ipc_data_is_valid (data);
        sat_status_break_on_error (status);

        sat_queue_data_internal_t __data = {.type = data->type};

        memset (__data.buffer, 0, SAT_QUEUE_DATA_BUFFER_SIZE);

        if (msgrcv (object->id, (void *)&__data, data->size, data->type, 0) == -1)
        {
            sat_status_set (&status, false, "sat queue ipc receive error: msgrcv failed");
            break;
        }

        memcpy (data->buffer, __data.buffer, data->size);

    } while (false);

    return status;
}

sat_status_t sat_queue_ipc_destroy (sat_queue_ipc_t *const object)
{
    sat_status_t status = sat_status_success (&status);

    do
    {
        if (object == NULL)
        {
            sat_status_set (&status, false, "sat queue ipc destroy error: null object");
            break;
        }

        if (object->initialized == false)
        {
            sat_status_set (&status, false, "sat queue ipc destroy error: not initialized");
            break;
        }

        if (msgctl (object->id, IPC_RMID, 0) != 0)
        {
            sat_status_set (&status, false, "sat queue ipc destroy error: msgctl failed");
        }

    } while (false);

    return status;
}

static sat_status_t sat_queue_ipc_args_is_valid (const sat_queue_ipc_args_t *const args)
{
    sat_status_t status = sat_status_success (&status);

    do
    {
        if (args == NULL)
        {
            sat_status_set (&status, false, "sat queue ipc args is valid error: null args");
            break;
        }

        if (args->key <= 0)
        {
            sat_status_set (&status, false, "sat queue ipc args is valid error: invalid key");
            break;
        }
    } while (false);

    return status;
}

static sat_status_t sat_queue_ipc_data_is_valid (const sat_queue_data_t *const data)
{
    sat_status_t status = sat_status_success (&status);

    do
    {
        if (data == NULL)
        {
            sat_status_set (&status, false, "sat queue ipc data is valid error: null data");
            break;
        }

        if (data->type <= 0)
        {
            sat_status_set (&status, false, "sat queue ipc data is valid error: invalid type");
            break;
        }

        if (data->buffer == NULL)
        {
            sat_status_set (&status, false, "sat queue ipc data is valid error: null buffer");
            break;
        }

        if (data->size == 0 ||
            data->size >= SAT_QUEUE_DATA_BUFFER_SIZE)
        {
            sat_status_set (&status, false, "sat queue ipc data is valid error: invalid size");
        }

    } while (false);

    return status;
}