#include <sat_discovery_handle_frames.h>
#include <stdio.h>

void sat_discovery_handle_frame_heartbeat (sat_discovery_t *const service, sat_discovery_frame_t frame)
{
    // verify if the server which is sending heartbeat is registered
    // and update its last seen timestamp
    uuid_string_t frame_uuid_string;

    sat_uuid_bin_to_string (frame.header.uuid, frame_uuid_string, sat_uuid_format_upper_case);

    printf ("Discovery heartbeat received: %s\n", frame_uuid_string);
}