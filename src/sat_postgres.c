#include <sat_postgres.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define SAT_POSTGRES_CONNECTION_STRING_SIZE 1024

static sat_status_t sat_postgres_is_args_valid (sat_postgres_args_t *args);
static sat_status_t sat_postgres_connect (sat_postgres_t *object);

sat_status_t sat_postgres_init (sat_postgres_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat postgres init error");

    if (object != NULL)
    {
        memset (object, 0, sizeof (sat_postgres_t));

        sat_status_set (&status, true, "");
    }

    return status;
}

sat_status_t sat_postgres_open (sat_postgres_t *object, sat_postgres_args_t *args)
{
    sat_status_t status = sat_status_set (&status, false, "sat postgres open error");

    if (object != NULL && args != NULL)
    {
        status = sat_postgres_is_args_valid (args);
        if (sat_status_get_result (&status) == true)
        {
            object->hostname = args->hostname;
            object->user     = args->user;
            object->password = args->password;
            object->database = args->database;
            object->port     = args->port;

            status = sat_postgres_connect (object);
        }
    }

    return status;
}

sat_status_t sat_postgres_execute (sat_postgres_t *object, const char *query)
{
    sat_status_t status = sat_status_set (&status, false, "sat postgres execute error");

    if (object != NULL && query != NULL)
    {
        if (object->result != NULL)
            PQclear (object->result);
        
        object->result = PQexec (object->connection, query);

        if (PQresultStatus (object->result) == PGRES_COMMAND_OK || PQresultStatus (object->result) == PGRES_TUPLES_OK)
            sat_status_set (&status, true, "");
    }

    return status;
}

sat_status_t sat_postgres_result_set (sat_postgres_t *object, sat_postgres_on_result_set_t on_result_set, void *user)
{
    sat_status_t status = sat_status_set (&status, false, "sat postgres result set error");

    if (object != NULL && on_result_set != NULL && user != NULL)
    {
        if (object->result != NULL)
        {
            int rows = PQntuples (object->result);
            int columns = PQnfields (object->result);

            char **data = (char **) malloc (sizeof (char *) * columns);

            for (int i = 0; i < rows; i++)
            {
                for (int j = 0; j < columns; j++)
                {
                    data [j] = PQgetvalue (object->result, i, j);
                }

                on_result_set (data, columns, user);
            }

            free (data);

            PQclear (object->result);
            object->result = NULL;

            sat_status_set (&status, true, "");
        }

    }

    return status;
}

sat_status_t sat_postgres_close (sat_postgres_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat postgres close error");

    if (object != NULL && object->connection != NULL)
    {
        if (object->result != NULL)
            PQclear (object->result);

        PQfinish (object->connection);

        memset (object, 0, sizeof (sat_postgres_t));

        sat_status_set (&status, true, "");
    }

    return status;
}

static sat_status_t sat_postgres_is_args_valid (sat_postgres_args_t *args)
{
    sat_status_t status = sat_status_set (&status, false, "sat postgres args error");

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

static sat_status_t sat_postgres_connect (sat_postgres_t *object)
{
    const char *format = "host=%s port=%d user=%s password=%s dbname=%s";
    char connection_string [SAT_POSTGRES_CONNECTION_STRING_SIZE] = {0};
    sat_status_t status = sat_status_set (&status, false, "sat postgres connect error");

    snprintf (connection_string, SAT_POSTGRES_CONNECTION_STRING_SIZE - 1, format, object->hostname,
                                                                                  object->port,
                                                                                  object->user,
                                                                                  object->password,
                                                                                  object->database);


    object->connection = PQconnectdb (connection_string);

    PQstatus (object->connection) == CONNECTION_OK ? sat_status_set (&status, true, "") : status;

    return status;
}