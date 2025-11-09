#ifndef SAT_LOKI_H
#define SAT_LOKI_H

#include <sat_status.h>

typedef struct 
{
    char *address;
    char *port;
    char *service_name;
    char *fallback;    
} sat_loki_args_t;


sat_status_t sat_loki_open (const sat_loki_args_t *const args);

sat_status_t sat_loki_send (const char *const level, const char *fmt, ...);

sat_status_t sat_loki_close (void);


#endif /* SAT_LOKI_H */
