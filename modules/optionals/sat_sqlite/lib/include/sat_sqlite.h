#ifndef SAT_SQLITE_H_
#define SAT_SQLITE_H_

#include <sat_status.h>
#include <sqlite3.h>

typedef struct 
{
    sqlite3 *connection;
    const char *database;
} sat_sqlite_t;

typedef struct 
{
    const char *database;
} sat_sqlite_args_t;

typedef int (*sat_sqlite_on_result_set_t) (void *user, int columns, char **data, char **names);

sat_status_t sat_sqlite_init (sat_sqlite_t *object);
sat_status_t sat_sqlite_open (sat_sqlite_t *object, sat_sqlite_args_t *args);
sat_status_t sat_sqlite_execute (sat_sqlite_t *object, const char *query, sat_sqlite_on_result_set_t on_result_set, void *data);
sat_status_t sat_sqlite_close (sat_sqlite_t *object);
 
#endif/* SAT_SQLITE_H_ */
