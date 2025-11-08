#include <sat_discovery_handle_frames.h>
#include <sat_discovery_frame.h>
#include <string.h>
#include <sat_log.h>
#include <sat_iterator.h>
#include <sat_network.h>

void sat_discovery_handle_frame_interest (sat_discovery_t *const discovery, sat_discovery_frame_t frame)
{
    // Respond to interest if service is offered

    sat_log_debug ("Received interest for service: %s", frame.payload.interest.service_name);

    if (strcmp (frame.payload.interest.service_name, discovery->service_name) == 0)
    {
        sat_iterator_t iter;
        sat_iterator_open (&iter, (sat_iterator_base_t *)discovery->interfaces);
        sat_network_info_t *interface = sat_iterator_next (&iter);
        while (interface != NULL)
        {
            sat_discovery_frame_t frame;
            sat_discovery_frame_buffer_t buffer = {0};
            sat_discovery_frame_create (&frame, &(sat_discovery_frame_args_t)
                                                {
                                                    .type = sat_discovery_frame_type_announce,
                                                    .uuid = &discovery->uuid,
                                                    .service_name = discovery->service_name,
                                                    .service_port = discovery->app_port,
                                                    .address = interface->ip_address
                                                });

            sat_log_debug ("Service %s announcing on address: %s", discovery->service_name, interface->ip_address);

            sat_discovery_frame_pack (&frame, buffer);

            sat_udp_send (&discovery->udp, (void *)&buffer, sizeof (buffer), &(sat_udp_destination_t)
                                                                            {
                                                                            .hostname = discovery->channel.address,
                                                                            .service = discovery->channel.service
                                                                            });
            
            
            interface = sat_iterator_next (&iter);
        }
    }
}