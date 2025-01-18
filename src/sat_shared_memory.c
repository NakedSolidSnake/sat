#include <sat_shared_memory.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/ipc.h>

static sat_status_t sat_shared_memory_is_args_valid (sat_shared_memory_args_t *args);

sat_status_t sat_shared_memory_open (sat_shared_memory_t *object, sat_shared_memory_args_t *args)
{
    sat_status_t status = sat_status_set (&status, false, "sat shared memory open error");

    if (object != NULL)
    {
        memset (object, 0, sizeof (sat_shared_memory_t));

        do 
        {
            status = sat_shared_memory_is_args_valid (args);
            if (sat_status_get_result (&status) == false)
                break;

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

            sat_status_set (&status, true, "");

        } while (false);
    }

    return status;
}

sat_status_t sat_shared_memory_get (sat_shared_memory_t *object, void **data)
{
    sat_status_t status = sat_status_set (&status, false, "sat shared memory get error");

    if (object != NULL && object->initialized == true && data != NULL)
    {
        *data = object->handle;

        sat_status_set (&status, true, "");
    }

    return status;
}

sat_status_t sat_shared_memory_detach (sat_shared_memory_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat shared memory detach error");

    if (object != NULL && object->initialized == true)
    {
        if (shmdt (object->handle) == 0)
            sat_status_set (&status, true, "");
    }

    return status;
}

sat_status_t sat_shared_memory_destroy (sat_shared_memory_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat shared memory destroy error");

    if (object != NULL && object->initialized == true)
    {
        if (shmctl (object->id, IPC_RMID, 0) == 0)
            sat_status_set (&status, true, "");
    }

    return status;
}

static sat_status_t sat_shared_memory_is_args_valid (sat_shared_memory_args_t *args)
{
    sat_status_t status = sat_status_set (&status, false, "sat shared memory args error");

    if (args != NULL &&
        args->size > 0)
    {
        sat_status_set (&status, true, "");
    }

    return status;
}