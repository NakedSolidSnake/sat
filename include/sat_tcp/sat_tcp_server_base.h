#ifndef SAT_TCP_SERVER_BASE_H_
#define SAT_TCP_SERVER_BASE_H_

#include <sat_status.h>

typedef struct 
{
    sat_status_t (*handle_client) (void *object, int client);
} sat_tcp_server_base_t;

#endif/* SAT_TCP_SERVER_BASE_H_ */
