#include <sat_log.h>
#include <string.h>
#include <stdio.h>

static sat_status_t  sat_log_set_logger_by (sat_log_t *object, sat_log_type_t type, sat_log_logger_t logger);
static void  sat_log_logger_none (sat_log_level_t level, const char *format, va_list list);
static void  sat_log_logger_default (sat_log_level_t level, const char *format, va_list list);

sat_status_t sat_log_init (sat_log_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat log init error");

    if (object != NULL)
    {
        memset (object, 0, sizeof (sat_log_t));
        sat_status_set (&status, true, "");
    }

    return status;
}

sat_status_t sat_log_open (sat_log_t *object, sat_log_args_t *args)
{
    sat_status_t status = sat_status_set (&status, false, "sat log open error");

    if (object != NULL && args != NULL)
    {
        object->type = args->type;

        status = sat_log_set_logger_by (object, args->type, args->logger);
    }

    return status;
}

sat_status_t sat_log_registry (sat_log_t *object, sat_log_level_t level, const char *format, ...)
{
    sat_status_t status = sat_status_set (&status, false, "sat log registry error");

    if (object != NULL && format != NULL)
    {
        va_list var_args;

        va_start (var_args, format);
        object->logger (level, format, var_args);
        va_end (var_args);

        sat_status_set (&status, true, "");
    }

    return status;
}

sat_status_t sat_log_close (sat_log_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat log close error");

    if (object != NULL)
    {
        memset (object, 0, sizeof (sat_log_t));
        sat_status_set (&status, true, "");
    }

    return status;
}

static sat_status_t  sat_log_set_logger_by (sat_log_t *object, sat_log_type_t type, sat_log_logger_t logger)
{
    sat_status_t status = sat_status_set (&status, true, "");

    switch (type)
    {
    case sat_log_type_none:
        object->logger = sat_log_logger_none;
        break;
    
    case sat_log_type_default:
        object->logger = sat_log_logger_default;
        break;
    
    case sat_log_type_custom:
        object->logger = logger;
        break;

    default:
        status = sat_status_set (&status, false, "sat log type error");
        break;
    }

    return status;
}

static void sat_log_logger_none (sat_log_level_t level, const char *format, va_list list)
{
    (void)level;
    (void)format;
    (void)list;
}

static void sat_log_logger_default (sat_log_level_t level, const char *format, va_list list)
{
    (void)level;
    vprintf (format, list);
}