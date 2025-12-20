#include <sat_shared_memory.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/ipc.h>

static sat_status_t sat_shared_memory_is_args_valid (const sat_shared_memory_args_t *const args);

sat_status_t sat_shared_memory_open (sat_shared_memory_t *const object, const sat_shared_memory_args_t *const args)
{
    sat_status_t status = sat_status_success (&status);

    do
    {
        if (object == NULL)
        {
            sat_status_set (&status, false, "sat shared memory open error: null object");
            break;
        }

        status = sat_shared_memory_is_args_valid (args);
        sat_status_break_on_error (status);

        memset (object, 0, sizeof (sat_shared_memory_t));

        object->id = shmget (args->key, args->size, args->flags | IPC_CREAT);
        if (object->id == -1)
        {
            sat_status_set (&status, false, "sat shared memory create error");
            break;
        }

        object->handle = shmat (object->id, (void *)0, 0);            
        if (object->handle == NULL)
        {
            sat_status_set (&status, false, "sat shared memory attach error");
            break;
        }

        object->key = args->key;
        object->size = args->size;

        object->initialized = true;

    } while (false);

    return status;
}

sat_status_t sat_shared_memory_get (const sat_shared_memory_t *const object, void **const data)
{
    sat_status_t status = sat_status_success (&status);

    do
    {
        if (object == NULL)
        {
            sat_status_set (&status, false, "sat shared memory get error: null object");
            break;
        }

        if (object->initialized == false)
        {
            sat_status_set (&status, false, "sat shared memory get error: not initialized");
            break;
        }

        if (data == NULL)
        {
            sat_status_set (&status, false, "sat shared memory get error: null data pointer");
            break;
        }

        *data = object->handle;

    } while (false);

    return status;
}

sat_status_t sat_shared_memory_detach (const sat_shared_memory_t *const object)
{
    sat_status_t status = sat_status_success (&status);

    do
    {
        if (object == NULL)
        {
            sat_status_set (&status, false, "sat shared memory detach error: null object");
            break;
        }

        if (object->initialized == false)
        {
            sat_status_set (&status, false, "sat shared memory detach error: not initialized");
            break;
        }

        if (shmdt (object->handle) != 0)
        {
            sat_status_set (&status, false, "sat shared memory detach error: shmdt failed");
            break;
        }

    } while (false);

    return status;
}

sat_status_t sat_shared_memory_destroy (sat_shared_memory_t *const object)
{
    sat_status_t status = sat_status_success (&status);

    do
    {
        if (object == NULL)
        {
            sat_status_set (&status, false, "sat shared memory destroy error: null object");
            break;
        }

        if (object->initialized == false)
        {
            sat_status_set (&status, false, "sat shared memory destroy error: not initialized");
            break;
        }

        if (shmctl (object->id, IPC_RMID, 0) != 0)
        {
            sat_status_set (&status, false, "sat shared memory destroy error: shmctl failed");
            break;
        }

        memset (object, 0, sizeof (sat_shared_memory_t));

    } while (false);

    return status;
}

static sat_status_t sat_shared_memory_is_args_valid (const sat_shared_memory_args_t *const args)
{
    sat_status_t status = sat_status_success (&status);

    do
    {
        if (args == NULL)
        {
            sat_status_set (&status, false, "sat shared memory args validation error: null args");
            break;
        }

        if (args->size == 0)
        {
            sat_status_set (&status, false, "sat shared memory args validation error: size is zero");
            break;
        }

    } while (false);

    return status;
}