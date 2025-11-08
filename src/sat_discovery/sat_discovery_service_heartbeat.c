#include <sat_discovery_services.h>
#include <sat_discovery.h>
#include <sat_discovery_frame.h>
#include <sat_log.h>

void sat_discovery_service_heartbeat (void *object)
{
    sat_discovery_t *discovery = (sat_discovery_t *)object;

    sat_log_debug ("Sending heartbeat for service: %s", discovery->service_name);
    
    sat_discovery_frame_t frame;
    sat_discovery_frame_buffer_t buffer = {0};

    sat_discovery_frame_create (&frame, &(sat_discovery_frame_args_t)
                                        {
                                            .type = sat_discovery_frame_type_heartbeat,
                                            .uuid = &discovery->uuid,
                                            .service_name = discovery->service_name,
                                        });

    sat_discovery_frame_pack (&frame, buffer);

    sat_udp_send (&discovery->udp, (void *)&buffer, sizeof (buffer), &(sat_udp_destination_t)
                                                                    {
                                                                    .hostname = discovery->channel.address,
                                                                    .service = discovery->channel.service
                                                                    });
}