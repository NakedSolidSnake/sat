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
    sat_status_return_on_null (object, "null object");
    sat_status_return_on_error (sat_queue_ipc_args_is_valid (args));

    memset (object, 0, sizeof (sat_queue_ipc_t));

    object->id = msgget ((key_t)args->key, args->flags | IPC_CREAT);
    sat_status_return_on_equals (object->id, -1, "msgget failed");

    object->initialized = true;
    object->key = args->key;

    sat_status_return_on_success ();
}

sat_status_t sat_queue_ipc_send (sat_queue_ipc_t *const object, const sat_queue_data_t *const data)
{
    sat_status_return_on_null (object, "null object");
    sat_status_return_on_false (object->initialized, "not initialized");
    sat_status_return_on_error (sat_queue_ipc_data_is_valid (data));

    sat_queue_data_internal_t __data = {.type = data->type};

    memset (__data.buffer, 0, SAT_QUEUE_DATA_BUFFER_SIZE);
    memcpy (__data.buffer, data->buffer, data->size);

    sat_status_return_on_equals (msgsnd (object->id, (void *)&__data, data->size, 0), -1, "msgsnd failed");

    sat_status_return_on_success ();
}

sat_status_t sat_queue_ipc_receive (sat_queue_ipc_t *const object, sat_queue_data_t *const data)
{
    sat_status_return_on_null (object, "null object");
    sat_status_return_on_false (object->initialized, "not initialized");
    sat_status_return_on_error (sat_queue_ipc_data_is_valid (data));

    sat_queue_data_internal_t __data = {.type = data->type};

    memset (__data.buffer, 0, SAT_QUEUE_DATA_BUFFER_SIZE);

    sat_status_return_on_equals (msgrcv (object->id, (void *)&__data, data->size, data->type, 0), -1, "msgrcv failed");

    memcpy (data->buffer, __data.buffer, data->size);

    sat_status_return_on_success ();
}

sat_status_t sat_queue_ipc_destroy (sat_queue_ipc_t *const object)
{
    sat_status_return_on_null (object, "null object");
    sat_status_return_on_false (object->initialized, "not initialized");

    sat_status_return_on_not_equals (msgctl (object->id, IPC_RMID, 0), 0, "msgctl failed");

    sat_status_return_on_success ();
}

static sat_status_t sat_queue_ipc_args_is_valid (const sat_queue_ipc_args_t *const args)
{
    sat_status_return_on_null (args, "null args");
    sat_status_return_on_less_than_or_equal (args->key, 0, "invalid key");

    sat_status_return_on_success ();
}

static sat_status_t sat_queue_ipc_data_is_valid (const sat_queue_data_t *const data)
{
    sat_status_return_on_null (data, "null data");
    sat_status_return_on_less_than_or_equal (data->type, 0, "invalid type");
    sat_status_return_on_null (data->buffer, "null buffer");
    sat_status_return_on_less_than_or_equal (data->size, 0, "invalid size");
    sat_status_return_on_greater_than_or_equal (data->size, SAT_QUEUE_DATA_BUFFER_SIZE, "invalid size");

    sat_status_return_on_success ();
}