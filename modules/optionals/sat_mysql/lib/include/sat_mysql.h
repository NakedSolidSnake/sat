#ifndef SAT_MYSQL_H_
#define SAT_MYSQL_H_

#include <sat_status.h>
#include <mysql/mysql.h>
#include <stdint.h>

typedef struct 
{
    MYSQL connection;
    const char *hostname;
    const char *user;
    const char *password;
    const char *database;
    uint16_t port;
} sat_mysql_t;

typedef struct 
{
    const char *hostname;
    const char *user;
    const char *password;
    const char *database;
    uint16_t port;
} sat_mysql_args_t;


typedef void (*sat_mysql_on_result_set_t) (char **data, int columns, void *user);

sat_status_t sat_mysql_init (sat_mysql_t *object);
sat_status_t sat_mysql_open (sat_mysql_t *object, sat_mysql_args_t *args);
sat_status_t sat_mysql_execute (sat_mysql_t *object, const char *query);
sat_status_t sat_mysql_result_set (sat_mysql_t *object, sat_mysql_on_result_set_t on_result_set, void *user);
sat_status_t sat_mysql_close (sat_mysql_t *object);

#endif/* SAT_MYSQL_H_ */
