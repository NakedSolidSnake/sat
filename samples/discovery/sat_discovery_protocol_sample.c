#include <sat_discovery/sat_discovery_frame.h>
#include <sat_uuid.h>
#include <stdio.h>

int main (void)
{
    sat_discovery_frame_t frame_in;
    sat_discovery_frame_t frame_out;
    sat_discovery_frame_args_t args;
    sat_discovery_frame_buffer_t buffer;
    sat_uuid_binary_t uuid;

    sat_uuid_generate_bin (uuid);

    args.type = sat_discovery_frame_type_announce;
    args.uuid = &uuid;
    args.service_name = "Test Service";
    args.service_port = "1234";
    args.address = "192.168.1.1";

    sat_discovery_frame_create(&frame_in, &args);
    sat_discovery_frame_print(&frame_in);

    putchar('\n');
    putchar('\n');

    sat_discovery_frame_pack(&frame_in, buffer);
    sat_discovery_frame_buffer_print(buffer);
    
    putchar('\n');
    putchar('\n');


    sat_discovery_frame_unpack(&frame_out, buffer);
    sat_discovery_frame_print(&frame_out);

    return 0;
}