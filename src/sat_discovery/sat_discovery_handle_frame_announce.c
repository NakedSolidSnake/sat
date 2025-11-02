#include <stdio.h>
#include <string.h>
#include <sat_discovery.h>
#include <sat_iterator.h>
#include <sat_discovery_node.h>
#include <sat_discovery_interest.h>
#include <sat_discovery_handle_frames.h>

void sat_discovery_handle_frame_announce (sat_discovery_t *const service, sat_discovery_frame_t frame)
{
    sat_iterator_t iterator;
    sat_status_t status = sat_iterator_open (&iterator, (sat_iterator_base_t *)service->interests);
    if (sat_status_get_result (&status) == true)
    {
        // printf ("Scanning interests for service: %s\n", frame.payload.announce.service_name);
        sat_discovery_interest_t *interest = sat_iterator_next (&iterator);
        while (interest != NULL)
        {
            if (interest->registered == true)
            {
                // printf ("Interest %s already registered\n", interest->name);
                break;
            }

            if (strcmp (interest->name, frame.payload.announce.service_name) == 0)
            {
                sat_discovery_node_t node;
                sat_discovery_node_args_t node_args = 
                {
                    .name = frame.payload.announce.service_name,
                    .address = "", // TODO: get address from frame or UDP source
                    .port = frame.payload.announce.service_port
                };

                status = sat_discovery_node_create (&node, &node_args);

                if (sat_status_get_result (&status) == true)
                {
                    status = sat_set_add (service->nodes, &node);
                    if (sat_status_get_result (&status) == true)
                    {
                        printf ("Registered discovery node: %s\n", node.name);
                        interest->registered = true;
                    }
                }
                break;
            }

            interest = sat_iterator_next (&iterator);
        }
    }
}