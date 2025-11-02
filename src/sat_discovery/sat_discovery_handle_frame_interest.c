#include <sat_discovery_handle_frames.h>
#include <sat_discovery_frame.h>
#include <string.h>

void sat_discovery_handle_frame_interest (sat_discovery_t *const discovery, sat_discovery_frame_t frame)
{
    // Respond to interest if service is offered
    if (strcmp (frame.payload.interest.service_name, discovery->service_name) == 0)
    {
        sat_discovery_frame_t frame;
        sat_discovery_frame_buffer_t buffer = {0};
        sat_discovery_frame_create (&frame, &(sat_discovery_frame_args_t)
                                            {
                                                .type = sat_discovery_frame_type_announce,
                                                .uuid = &discovery->uuid,
                                                .service_name = discovery->service_name,
                                                .service_port = 0,
                                                .address = 0
                                            });


        sat_discovery_frame_pack (&frame, buffer);

        sat_udp_send (&discovery->udp, (void *)&buffer, sizeof (buffer), &(sat_udp_destination_t)
                                                                        {
                                                                            .hostname = discovery->channel.address,
                                                                            .service = discovery->channel.service
                                                                        });
    }
}