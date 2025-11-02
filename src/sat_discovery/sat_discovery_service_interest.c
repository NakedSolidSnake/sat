#include <sat_discovery_services.h>
#include <sat_discovery_frame.h>
#include <sat_discovery.h>
#include <sat_iterator.h>
#include <sat_discovery_interest.h>


void sat_discovery_service_interest (void *object)
{
    sat_discovery_t *discovery = (sat_discovery_t *)object;

    sat_iterator_t iterator;

    sat_status_t status = sat_iterator_open (&iterator, (sat_iterator_base_t *)discovery->interests);
    if (sat_status_get_result (&status) == true)
    {
        sat_discovery_interest_t *interest = sat_iterator_next (&iterator);
        while (interest != NULL && interest->registered == false)
        {
            sat_discovery_frame_t frame;
            sat_discovery_frame_buffer_t buffer = {0};
            sat_discovery_frame_create (&frame, &(sat_discovery_frame_args_t)
                                                {
                                                    .type = sat_discovery_frame_type_interest,
                                                    .uuid = &discovery->uuid,
                                                    .service_name = interest->name,
                                                    .service_port = 0,
                                                    .address = 0
                                                });

            // sat_discovery_frame_print (&frame);

            sat_discovery_frame_pack (&frame, buffer);


            // Send interest message
            sat_udp_send (&discovery->udp, (void *)&buffer, sizeof (buffer), &(sat_udp_destination_t)
                                                                    {
                                                                    .hostname = discovery->channel.address,
                                                                    .service = discovery->channel.service
                                                                    });
            interest = sat_iterator_next (&iterator);
        }
    }
}