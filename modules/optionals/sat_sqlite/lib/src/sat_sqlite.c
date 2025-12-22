#include <sat_sqlite.h>
#include <string.h>

static sat_status_t sat_sqlite_is_args_valid (sat_sqlite_args_t *args);
static sat_status_t sat_sqlite_connect (sat_sqlite_t *object);

sat_status_t sat_sqlite_init (sat_sqlite_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat sqlite init error");

    if (object != NULL)
    {
        memset (object, 0, sizeof (sat_sqlite_t));

        sat_status_set (&status, true, "");
    }

    return status;
}

sat_status_t sat_sqlite_open (sat_sqlite_t *object, sat_sqlite_args_t *args)
{
    sat_status_t status = sat_status_set (&status, false, "sat sqlite open error");

    if (object != NULL && args != NULL)
    {
        status = sat_sqlite_is_args_valid (args);
        if (sat_status_get_result (&status) == true)
        {
            object->database = args->database;

            status = sat_sqlite_connect (object);
        }
    }

    return status;
}

sat_status_t sat_sqlite_execute (sat_sqlite_t *object, const char *query, sat_sqlite_on_result_set_t on_result_set, void *data)
{
    sat_status_t status = sat_status_set (&status, false, "sat sqlite execute error");

    if (object != NULL && query != NULL)
    {
        sqlite3_exec (object->connection, query, on_result_set, data, NULL) == SQLITE_OK ? sat_status_set (&status, true, "") : status;
    }

    return status;
}

sat_status_t sat_sqlite_close (sat_sqlite_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat sqlite close error");

    if (object != NULL)
    {
        sqlite3_close (object->connection);

        memset (object, 0, sizeof (sat_sqlite_t));

        sat_status_set (&status, true, "");
    }

    return status;
}

static sat_status_t sat_sqlite_is_args_valid (sat_sqlite_args_t *args)
{
    sat_status_t status = sat_status_set (&status, false, "sat sqlite args error");

    if (args->database != NULL)
    {
        sat_status_set (&status, true, "");
    }

    return status;
}

static sat_status_t sat_sqlite_connect (sat_sqlite_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat sqlite connect error");

    int result = sqlite3_open (object->database, &object->connection);

    (result == SQLITE_OK) ? sat_status_set (&status, true, "") : status;

    return status;
}