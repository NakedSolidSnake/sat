#include <sat_mysql.h>
#include <string.h>

static sat_status_t sat_mysql_is_args_valid (sat_mysql_args_t *args);
static sat_status_t sat_mysql_connect (sat_mysql_t *object);

sat_status_t sat_mysql_init (sat_mysql_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat mysql init error");

    if (object != NULL)
    {
        memset (object, 0, sizeof (sat_mysql_t));

        sat_status_set (&status, true, "");
    }

    return status;
}

sat_status_t sat_mysql_open (sat_mysql_t *object, sat_mysql_args_t *args)
{
    sat_status_t status = sat_status_set (&status, false, "sat mysql open error");

    if (object != NULL && args != NULL)
    {
        status = sat_mysql_is_args_valid (args);
        if (sat_status_get_result (&status) == true)
        {
            object->hostname = args->hostname;
            object->user     = args->user;
            object->password = args->password;
            object->database = args->database;
            object->port     = args->port;

            status = sat_mysql_connect (object);
        }

        sat_status_set (&status, true, "");
    }

    return status;
}

sat_status_t sat_mysql_execute (sat_mysql_t *object, const char *query)
{
    sat_status_t status = sat_status_set (&status, false, "sat mysql execute error");

    if (object != NULL && query != NULL)
    {
        mysql_query (&object->connection, query) ? status : sat_status_set (&status, true, "");
    }

    return status;
}

sat_status_t sat_mysql_result_set (sat_mysql_t *object, sat_mysql_on_result_set_t on_result_set, void *user)
{
    sat_status_t status = sat_status_set (&status, false, "sat mysql result set error");

    if (object != NULL && on_result_set != NULL && user != NULL)
    {

        MYSQL_RES *result = mysql_store_result (&object->connection);

        if (result != NULL)
        {
            MYSQL_ROW row;

            int fields = mysql_num_fields (result);

            while ((row = mysql_fetch_row (result)))
            {
                on_result_set (row, fields, user);
            }

            mysql_free_result (result);

            sat_status_set (&status, true, "");
        }

    }

    return status;
}

sat_status_t sat_mysql_close (sat_mysql_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat mysql close error");

    if (object != NULL)
    {
        mysql_close (&object->connection);

        memset (object, 0, sizeof (sat_mysql_t));

        sat_status_set (&status, true, "");
    }

    return status;
}

static sat_status_t sat_mysql_is_args_valid (sat_mysql_args_t *args)
{
    sat_status_t status = sat_status_set (&status, false, "sat mysql args error");

    if (args->hostname != NULL &&
        args->user != NULL &&
        args->password != NULL &&
        args->database != NULL &&
        args->port > 0
        )
    {
        sat_status_set (&status, true, "");
    }

    return status;
}

static sat_status_t sat_mysql_connect (sat_mysql_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat mysql connect error");

    MYSQL *result = NULL;

    mysql_init (&object->connection);

    result = mysql_real_connect (&object->connection,
                                 object->hostname,
                                 object->user,
                                 object->password,
                                 object->database,
                                 object->port,
                                 NULL,
                                 0);

    (result == NULL) ? status : sat_status_set (&status, true, "");

    return status;
}