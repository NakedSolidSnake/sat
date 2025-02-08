#ifndef SAT_UDP_SERVER_INTERACTIVE_H_
#define SAT_UDP_SERVER_INTERACTIVE_H_

#include <sat_udp_server_base.h>
#include <sat_udp_server_abstract.h>

typedef struct 
{
    sat_udp_server_base_t base;
    sat_udp_server_abstract_t abstract;
    
} sat_udp_server_interactive_t;

sat_udp_server_base_t *sat_udp_server_interactive_create (void);

#endif/* SAT_UDP_SERVER_INTERACTIVE_H_ */
