#include <sat_discovery_services.h>
#include <sat_discovery.h>
#include <sat_discovery_node.h>
#include <sat_time.h>
#include <sat_iterator.h>
#include <sat_discovery_interest.h>
#include <stdio.h>
#include <string.h>

#define SAT_DISCOVERY_NODE_HEARTBEAT_TIMEOUT_S 10

/* Remove when a set add the remove by value function */
static int32_t find_by_name (sat_set_t *set, const char *name);

void sat_discovery_service_node_ageing (void *object)
{
    sat_discovery_t *discovery = (sat_discovery_t *)object;

    sat_iterator_t iterator;

    sat_status_t status = sat_iterator_open (&iterator, (sat_iterator_base_t *)discovery->nodes);
    if (sat_status_get_result (&status) == true)
    {
        sat_discovery_node_t *node = sat_iterator_next (&iterator);
        while (node != NULL)
        {
            if (sat_time_get_utc_epoch_now () - node->last_heartbeat > SAT_DISCOVERY_NODE_HEARTBEAT_TIMEOUT_S)
            {
                printf ("Node %s timed out, removing...\n", node->name);
                int32_t index = find_by_name (discovery->nodes, node->name);
                if (index >= 0)
                {
                    sat_iterator_t interest_iterator;
                    status = sat_iterator_open (&interest_iterator, (sat_iterator_base_t *)discovery->interests);
                    if (sat_status_get_result (&status) == true)
                    {
                        sat_discovery_interest_t *interest = sat_iterator_next (&interest_iterator);
                        while (interest != NULL)
                        {
                            if (strcmp (interest->name, node->name) == 0)
                            {
                                // Mark interest as unregistered
                                printf ("Marking interest %s as unregistered\n", interest->name);
                                interest->registered = false;
                                break;
                            }

                            interest = sat_iterator_next (&interest_iterator);
                        }
                    }

                    sat_set_remove_by (discovery->nodes, index);
                }
            }

            node = sat_iterator_next (&iterator);
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