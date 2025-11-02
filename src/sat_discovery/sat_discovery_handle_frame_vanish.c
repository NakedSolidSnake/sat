#include <sat_discovery_handle_frames.h>
#include <stdio.h>

void sat_discovery_handle_frame_vanish (sat_discovery_t *const service, sat_discovery_frame_t frame)
{
    printf ("Discovery interest received: %s\n", frame.payload.interest.service_name);
}