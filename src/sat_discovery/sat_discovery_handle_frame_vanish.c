#include <sat_discovery_handle_frames.h>
#include <sat_discovery_node.h>
#include <sat_discovery_interest.h>
#include <sat_iterator.h>
#include <sat_uuid.h>
#include <stdio.h>
#include <string.h>

static int32_t find_by_name (sat_set_t *set, const char *name);

void sat_discovery_handle_frame_vanish (sat_discovery_t *const service, sat_discovery_frame_t frame)
{
    int32_t index = find_by_name (service->nodes, frame.payload.vanish.service_name);

    if (index >= 0)
    {
        sat_iterator_t iterator;
        sat_status_t status = sat_iterator_open (&iterator, (sat_iterator_base_t *)service->interests);
        if (sat_status_get_result (&status) == true)
        {
            sat_discovery_interest_t *interest = sat_iterator_next (&iterator);
            while (interest != NULL)
            {
                if (strcmp (interest->name, frame.payload.vanish.service_name) == 0)
                {
                    // Mark interest as unregistered
                    sat_set_remove_by (service->nodes, index);

                    interest->registered = false;
                    break;
                }

                interest = sat_iterator_next (&iterator);
            }
        }   
    }
}

static int32_t find_by_name (sat_set_t *set, const char *name)
{
    int32_t index = -1;

    sat_iterator_t iterator;
    sat_status_t status = sat_iterator_open (&iterator, (sat_iterator_base_t *)set);
    if (sat_status_get_result (&status) == true)
    {
        sat_discovery_node_t *node = sat_iterator_next (&iterator);
        uint32_t current_index = 0;
        while (node != NULL)
        {
            if (strcmp (node->name, name) == 0)
            {
                index = current_index;
                break;
            }

            node = sat_iterator_next (&iterator);
            current_index++;
        }
    }

    return index;
}