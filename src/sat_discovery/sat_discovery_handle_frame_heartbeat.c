#include <sat_discovery_handle_frames.h>
#include <sat_discovery_node.h>
#include <sat_discovery_interest.h>
#include <sat_iterator.h>
#include <stdio.h>
#include <string.h>

void sat_discovery_handle_frame_heartbeat (sat_discovery_t *const service, sat_discovery_frame_t frame)
{
    // verify if the server which is sending heartbeat is registered
    // and update its last seen timestamp
    // uuid_string_t frame_uuid_string;

    // sat_uuid_bin_to_string (frame.header.uuid, frame_uuid_string, sat_uuid_format_upper_case);
    // printf ("Discovery heartbeat received: %s\n", frame_uuid_string);

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
                // printf ("Heartbeat for registered service: %s\n", interest->name);
                // sat_discovery_interest_update_last_seen (interest);
                sat_iterator_t iterator;
                sat_status_t status = sat_iterator_open (&iterator, (sat_iterator_base_t *)service->nodes);
                if (sat_status_get_result (&status) == true)
                {
                    sat_discovery_node_t *node = sat_iterator_next (&iterator);
                    while (node != NULL)
                    {
                        // sat_discovery_node_debug (node);
                        // sat_discovery_frame_debug (&frame);

                        if (sat_uuid_compare_bin (frame.header.uuid, node->uuid) == true)
                        {
                            sat_discovery_node_update_last_seen (node);
                            // printf ("Updated heartbeat for node: %s\n", node->name);
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