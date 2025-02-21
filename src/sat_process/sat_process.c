#include <sat_process.h>
#include <string.h>
#include <stdint.h>

static void sat_process_spawn_by (sat_process_t *object);
static void sat_process_spawn_no_args (sat_process_t *object);
static void sat_process_spawn_with_args (sat_process_t *object);

static uint16_t sat_process_get_args_amount (sat_process_t *object);

static void process_get_args_list (sat_process_t *object, char **args_list);

sat_status_t sat_process_create (sat_process_t *object, sat_process_args_t *args)
{
    sat_status_t status = sat_status_set (&status, false, "sat process create error");

    if (object != NULL && args != NULL && args->name && strlen (args->name) > 0)
    {
        memset (object, 0, sizeof (sat_process_t));

        strncpy (object->name, args->name, SAT_PROCESS_NAME_SIZE);

        if (args->args != NULL && strlen (args->args) > 0)
        {
            strncpy (object->args, args->args, SAT_PROCESS_ARGS_SIZE);
            object->type = sat_process_spawn_type_with_args;
        }

        object->initialized = true;
        object->mode = args->mode;

        sat_status_set (&status, true, "");
    }

    return status;
}

sat_status_t sat_process_spawn (sat_process_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat process spawn error");

    if (object != NULL && object->initialized == true)
    {
        // sat_status_set (&status, true, "");
        pid_t pid = fork ();

        switch (pid)
        {
            case -1:
                sat_status_set (&status, false, "sat process spawn child error");
            break;

            case 0:
                sat_process_spawn_by (object);
            break;

            default:
                object->pid = pid;
                sat_status_set (&status, true, "");
            break;
        }
    }

    return status;
}

static void sat_process_spawn_by (sat_process_t *object)
{
    if (object->type == sat_process_spawn_type_no_args)
    {
        sat_process_spawn_no_args (object);
    }

    else 
    {
        sat_process_spawn_with_args (object);
    }
}

static void sat_process_spawn_no_args (sat_process_t *object)
{
    execl (object->name, object->name, NULL);
}

static void sat_process_spawn_with_args (sat_process_t *object)
{
    uint16_t amount = sat_process_get_args_amount (object);
    char *args_list [amount + 1];

    process_get_args_list (object, args_list);

    execv (object->name, args_list);
}

static uint16_t sat_process_get_args_amount (sat_process_t *object)
{
    uint16_t amount = 1;
    char *args = object->args;

    for (uint16_t i = 0; i < strlen (args); i ++)
    {
        if (args [i] == ' ' && (args [i + 1] != '\0' && args [i + 1] != ' '))
        {
            amount++;
        }
    }

    return amount;
}

static void process_get_args_list (sat_process_t *object, char **args_list)
{
    uint16_t i = 1;

    args_list [0] = object->name;

    char *p = strtok (object->args, " ");
    while (p != NULL)
    {
        args_list [i] = p;
        i ++;
        p = strtok (NULL, " ");
    }

    args_list [i] = NULL;
}