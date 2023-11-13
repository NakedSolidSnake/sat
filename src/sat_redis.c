#include <sat_redis.h>
#include <stdio.h>
#include <string.h>
#include <hiredis/hiredis.h>

sat_status_t sat_redis_init (sat_redis_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat redis init error");

    if (object != NULL)
    {
        memset (object, 0, sizeof (sat_redis_t));
        sat_status_set (&status, true, "");
    }

    return status;
}

sat_status_t sat_redis_open (sat_redis_t *object, sat_redis_args_t *args)
{
    sat_status_t status = sat_status_set (&status, false, "sat redis open error");

    if (object != NULL && args != NULL && args->host != NULL)
    {
        sat_status_set (&status, false, "sat redis connect error");

        object->handle = redisConnect (args->host, args->port);

        if (object->handle != NULL && ((redisContext *)object->handle)->err == 0)
            sat_status_set (&status, true, "");
    }

    return status;
}

sat_status_t sat_redis_save (sat_redis_t *object, char *key, char *value)
{
    sat_status_t status = sat_status_set (&status, false, "sat redis save error");

    if (object != NULL && key != NULL && value != NULL)
    {
        redisReply *reply = redisCommand (object->handle, "SET %s %s", key, value);
        freeReplyObject (reply);

        sat_status_set (&status, true, "");
    }

    return status;    
}

sat_status_t sat_redis_get (sat_redis_t *object, char *key, sat_redis_on_read_t on_read, void *data)
{
    sat_status_t status = sat_status_set (&status, false, "sat redis get error");

    if (object != NULL && key != NULL && on_read != NULL && data != NULL)
    {
        redisReply *reply = redisCommand (object->handle, "GET %s", key);

        on_read (reply->str, data);

        freeReplyObject (reply);

        sat_status_set (&status, true, "");
    }

    return status;
}

sat_status_t sat_redis_is_key_exists (sat_redis_t *object, char *key)
{
    sat_status_t status = sat_status_set (&status, false, "sat redis is key exists error");

    if (object != NULL && key != NULL)
    {
        redisReply *reply = redisCommand (object->handle, "EXISTS %s", key);

        if (reply->integer == 1)
        {
            sat_status_set (&status, true, "");
        }

        freeReplyObject (reply);
    }

    return status;
}

sat_status_t sat_redis_close (sat_redis_t *object)
{
     sat_status_t status = sat_status_set (&status, false, "sat redis close error");

    if (object != NULL)
    {
        redisFree (object->handle);

        memset (object, 0, sizeof (sat_redis_t));

        sat_status_set (&status, true, "");
    }

    return status;
}