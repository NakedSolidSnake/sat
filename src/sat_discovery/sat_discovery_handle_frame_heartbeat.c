#include <sat_discovery_handle_frames.h>
#include <sat_discovery_node.h>
#include <sat_discovery_interest.h>
#include <sat_iterator.h>
#include <stdio.h>
#include <string.h>
#include <sat_log.h>

void sat_discovery_handle_frame_heartbeat (sat_discovery_t *const service, sat_discovery_frame_t frame)
{
    // verify if the server which is sending heartbeat is registered
    // and update its last seen timestamp

    sat_log_debug ("Received heartbeat for service: %s", frame.payload.heartbeat.service_name);

    sat_iterator_t iterator;
    
    sat_status_t status = sat_iterator_open (&iterator, (sat_iterator_base_t *)service->interests);
    if (sat_status_get_result (&status) == true)
    {
        sat_discovery_interest_t *interest = sat_iterator_next (&iterator);
        while (interest != NULL)
        {
            if (strcmp (interest->name, frame.payload.heartbeat.service_name) == 0 
                && interest->registered == true)
            {
                sat_iterator_t iterator;

                sat_status_t status = sat_iterator_open (&iterator, (sat_iterator_base_t *)service->nodes);
                if (sat_status_get_result (&status) == true)
                {
                    sat_discovery_node_t *node = sat_iterator_next (&iterator);
                    while (node != NULL)
                    {

                        if (sat_uuid_compare_bin (frame.header.uuid, node->uuid) == true)
                        {
                            sat_discovery_node_update_last_seen (node);

                            break;
                        }
                        
                        node = sat_iterator_next (&iterator);
                    }
                }  

                break;
            }   

            interest = sat_iterator_next (&iterator);
        }
    }
}