#include <sat_discovery_handle_frames.h>
#include <stdio.h>

void sat_discovery_handle_frame_interest (sat_discovery_t *const service, sat_discovery_frame_t frame)
{
    // Respond to interest if service is offered

    printf ("Discovery interest received: %s\n", frame.payload.interest.service_name);
}