#include <sat_process.h>
#include <string.h>
#include <stdint.h>

static void sat_process_spawn_by (const sat_process_t *const object);
static void sat_process_spawn_no_args (const sat_process_t *const object);
static void sat_process_spawn_with_args (const sat_process_t *const object);

static uint16_t sat_process_get_args_amount (const sat_process_t *const object);
static void process_get_args_list (const sat_process_t *const object, char **const args_list);

sat_status_t sat_process_create (sat_process_t *const object, const sat_process_args_t *const args)
{
    sat_status_return_on_null (object, "sat process create error: null object");
    sat_status_return_on_null (args, "sat process create error: null args");
    sat_status_return_on_equals (strlen (args->name), 0, "sat process create error: name is zeroed");

    memset (object, 0, sizeof (sat_process_t));

    strncpy (object->name, args->name, SAT_PROCESS_NAME_SIZE);

    if (strlen (args->args) > 0)
    {
        strncpy (object->args, args->args, SAT_PROCESS_ARGS_SIZE);
        object->type = sat_process_spawn_type_with_args;
    }

    object->initialized = true;
    object->mode = args->mode;

    sat_status_return_on_success ();
}

sat_status_t sat_process_spawn (sat_process_t *object)
{
    sat_status_return_on_null (object, "sat process spawn error: null object");
    sat_status_return_on_false (object->initialized, "sat process spawn error: object not initialized");

    pid_t pid = fork ();

    switch (pid)
    {
        case -1:
            sat_status_return_on_failure ("sat process spawn child error");
        break;

        case 0:
            sat_process_spawn_by (object);
        break;

        default:
            object->pid = pid;            
        break;
    }

    sat_status_return_on_success ();
}

static void sat_process_spawn_by (const sat_process_t *const object)
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

static void sat_process_spawn_no_args (const sat_process_t *const object)
{
    execl (object->name, object->name, NULL);
}

static void sat_process_spawn_with_args (const sat_process_t *const object)
{
    uint16_t amount = sat_process_get_args_amount (object);
    char *args_list [amount + 1];

    process_get_args_list (object, args_list);

    execv (object->name, args_list);
}

static uint16_t sat_process_get_args_amount (const sat_process_t *const object)
{
    uint16_t amount = 1;
    const char *const args = object->args;

    for (uint16_t i = 0; i < strlen (args); i ++)
    {
        if (args [i] == ' ' && (args [i + 1] != '\0' && args [i + 1] != ' '))
        {
            amount++;
        }
    }

    return amount;
}

static void process_get_args_list (const sat_process_t *const object, char **const args_list)
{
    uint16_t i = 1;

    args_list [0] = (char *const)object->name;

    char *p = strtok ((char *)object->args, " ");
    while (p != NULL)
    {
        args_list [i] = p;
        i ++;
        p = strtok (NULL, " ");
    }

    args_list [i] = NULL;
}