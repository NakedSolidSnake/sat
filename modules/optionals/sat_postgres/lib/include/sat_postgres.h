#ifndef SAT_POSTGRES_H_
#define SAT_POSTGRES_H_

#include <sat_status.h>
#include <postgresql/libpq-fe.h>
#include <stdint.h>

typedef struct 
{
    PGconn *connection;
    PGresult *result;
    const char *hostname;
    const char *user;
    const char *password;
    const char *database;
    uint16_t port;
} sat_postgres_t;

typedef struct 
{
    const char *hostname;
    const char *user;
    const char *password;
    const char *database;
    uint16_t port;
} sat_postgres_args_t;


typedef void (*sat_postgres_on_result_set_t) (char **data, int columns, void *user);

sat_status_t sat_postgres_init (sat_postgres_t *object);
sat_status_t sat_postgres_open (sat_postgres_t *object, sat_postgres_args_t *args);
sat_status_t sat_postgres_execute (sat_postgres_t *object, const char *query);
sat_status_t sat_postgres_result_set (sat_postgres_t *object, sat_postgres_on_result_set_t on_result_set, void *user);
sat_status_t sat_postgres_close (sat_postgres_t *object);

#endif/* SAT_POSTGRES_H_ */
