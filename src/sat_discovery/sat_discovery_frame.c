#include <sat_discovery_frame.h>
#include <string.h>
#include <arpa/inet.h>
#include <sat_time.h>
#include <stddef.h>
#include <endian.h>
#include <stdio.h>

#define SAT_DISCOVERY_FRAME_VERSION 1 

sat_status_t sat_discovery_frame_create (sat_discovery_frame_t *const object, sat_discovery_frame_args_t *args)
{
    sat_status_t status;

    do
    {
        if (object == NULL || args == NULL)
        {
            sat_status_failure (&status, "Invalid parameters");
            break;
        }

        // Clear the object memory
        memset (object, 0, sizeof (sat_discovery_frame_t));

        // Copy values from args to object
        object->header.version = SAT_DISCOVERY_FRAME_VERSION;
        object->header.type = args->type;
        memcpy (object->header.uuid, args->uuid, SAT_UUID_BINARY_SIZE);

        if (args->type < sat_discovery_frame_type_announce || args->type > sat_discovery_frame_type_vanish)
        {
            sat_status_failure (&status, "Invalid frame type");
            break;
        }

        switch (args->type)
        {
            case sat_discovery_frame_type_announce:
                // object->payload.announce.service_port = args->service_port;
                // object->payload.announce.address = args->address;
                strncpy (object->payload.announce.service_name, args->service_name, SAT_DISCOVERY_FRAME_SERVICE_NAME_SIZE - 1);
                object->payload.announce.service_name [SAT_DISCOVERY_FRAME_SERVICE_NAME_SIZE - 1] = '\0';
                break;

            case sat_discovery_frame_type_interest:
                strncpy (object->payload.interest.service_name, args->service_name, SAT_DISCOVERY_FRAME_SERVICE_NAME_SIZE - 1);
                object->payload.interest.service_name [SAT_DISCOVERY_FRAME_SERVICE_NAME_SIZE - 1] = '\0';
                break;

            // case sat_discovery_frame_type_heartbeat:
            //     strncpy (object->payload.heartbeat.service_name, args->service_name, SAT_DISCOVERY_FRAME_SERVICE_NAME_SIZE - 1);
            //     object->payload.heartbeat.service_name [SAT_DISCOVERY_FRAME_SERVICE_NAME_SIZE - 1] = '\0';
            //     object->payload.heartbeat.timestamp = sat_time_get_epoch_now_ms ();
            //     break;

            // case sat_discovery_frame_type_vanish:
            //     strncpy (object->payload.vanish.service_name, args->service_name, SAT_DISCOVERY_FRAME_SERVICE_NAME_SIZE - 1);
            //     object->payload.vanish.service_name [SAT_DISCOVERY_FRAME_SERVICE_NAME_SIZE - 1] = '\0';
            //     break;

            default:
                sat_status_failure (&status, "Invalid frame type");
                break;
        }

        sat_status_success (&status);

    } while (false);

    return status;
}

sat_status_t sat_discovery_frame_pack (const sat_discovery_frame_t *const object, sat_discovery_frame_buffer_t buffer)
{
    sat_status_t status;

    do
    {
        if (object == NULL || buffer == NULL)
        {
            sat_status_failure (&status, "Invalid parameters");
            break;
        }

        // Validate frame type before processing
        if (object->header.type < sat_discovery_frame_type_announce || object->header.type > sat_discovery_frame_type_vanish)
        {
            sat_status_failure (&status, "Invalid frame type");
            break;
        }

        // Verify buffer size can accommodate the frame
        if (sizeof (sat_discovery_frame_t) > SAT_DISCOVERY_FRAME_SIZE)
        {
            sat_status_failure (&status, "Frame size exceeds buffer capacity");
            break;
        }
        
        buffer [0] = object->header.version;
        buffer [1] = object->header.type;
        memcpy (&buffer [2], object->header.uuid, SAT_UUID_BINARY_SIZE);

        // Copy the frame data into the buffer
        // memcpy (buffer->data, object, sizeof (sat_discovery_frame_t));

        // Convert multi-byte fields to network byte order
        // uint16_t *service_port_ptr = NULL;
        // uint32_t *address_ptr = NULL;
        // uint64_t *timestamp_ptr = NULL;

        switch (object->header.type)
        {
            case sat_discovery_frame_type_announce:
                memcpy (&buffer [sizeof (sat_discovery_frame_header_t)], object->payload.announce.service_name, SAT_DISCOVERY_FRAME_SERVICE_NAME_SIZE);
                // service_port_ptr = (uint16_t *) &buffer [sizeof (sat_discovery_frame_header_t) + offsetof (sat_discovery_frame_announce_t, service_port)];
                // *service_port_ptr = htons (*service_port_ptr);

                // address_ptr = (uint32_t *) &buffer [sizeof (sat_discovery_frame_header_t) + offsetof (sat_discovery_frame_announce_t, address)];
                // *address_ptr = htonl (*address_ptr);
                break;

            case sat_discovery_frame_type_interest:
                // No multi-byte fields to convert
                memcpy (&buffer [sizeof (sat_discovery_frame_header_t)], object->payload.interest.service_name, SAT_DISCOVERY_FRAME_SERVICE_NAME_SIZE);
                break;

            case sat_discovery_frame_type_heartbeat:
                be64toh (object->payload.heartbeat.timestamp);
                // timestamp_ptr = (uint64_t *) &buffer [sizeof (sat_discovery_frame_header_t) + offsetof (sat_discovery_frame_heartbeat_t, timestamp)];
                // *timestamp_ptr = htobe64 (*timestamp_ptr);
                break;

            case sat_discovery_frame_type_vanish:
                // No multi-byte fields to convert
                break;

            default:
                // This should never be reached due to validation above
                sat_status_failure (&status, "Invalid frame type");
                break;
        }

        // Only set success if we didn't encounter any errors
        sat_status_success (&status);

    } while (false);

    return status;
}

sat_status_t sat_discovery_frame_unpack (sat_discovery_frame_t *const object, const sat_discovery_frame_buffer_t buffer)
{
    sat_status_t status;

    do
    {
        if (object == NULL || buffer == NULL)
        {
            sat_status_failure (&status, "Invalid parameters");
            break;
        }

        // Copy the frame data from the buffer
        // memcpy (object, buffer->data, sizeof (sat_discovery_frame_t));
        object->header.version = buffer [0];
        object->header.type = buffer[1];
        memcpy (object->header.uuid, &buffer [2], SAT_UUID_BINARY_SIZE);

            
        // Validate frame version
        if (object->header.version != SAT_DISCOVERY_FRAME_VERSION)
        {
            sat_status_failure (&status, "Invalid frame version");
            break;
        }

        // Validate frame type
        if (object->header.type < sat_discovery_frame_type_announce || object->header.type > sat_discovery_frame_type_vanish)
        {
            sat_status_failure (&status, "Invalid frame type");
            break;
        }

        // Convert multi-byte fields from network byte order to host byte order
        switch (object->header.type)
        {
            case sat_discovery_frame_type_announce:
                strncpy (object->payload.announce.service_name, &buffer [18], SAT_DISCOVERY_FRAME_SERVICE_NAME_SIZE - 1);
                object->payload.announce.service_port = ntohs (object->payload.announce.service_port);
                object->payload.announce.address = ntohl (object->payload.announce.address);
                break;

            case sat_discovery_frame_type_interest:
                // No multi-byte fields to convert
                strncpy (object->payload.interest.service_name, &buffer [18], SAT_DISCOVERY_FRAME_SERVICE_NAME_SIZE - 1);
                break;

            case sat_discovery_frame_type_heartbeat:
                object->payload.heartbeat.timestamp = be64toh (object->payload.heartbeat.timestamp);
                break;

            case sat_discovery_frame_type_vanish:
                // No multi-byte fields to convert
                break;

            default:
                // This should never be reached due to validation above
                sat_status_failure (&status, "Invalid frame type");
                break;
        }

        // Ensure service names are null-terminated for safety
        switch (object->header.type)
        {
            case sat_discovery_frame_type_announce:
                object->payload.announce.service_name[SAT_DISCOVERY_FRAME_SERVICE_NAME_SIZE - 1] = '\0';
                break;

            case sat_discovery_frame_type_interest:
                object->payload.interest.service_name[SAT_DISCOVERY_FRAME_SERVICE_NAME_SIZE - 1] = '\0';
                break;

            case sat_discovery_frame_type_heartbeat:
                break;

            case sat_discovery_frame_type_vanish:
                // object->payload.vanish.service_name[SAT_DISCOVERY_FRAME_SERVICE_NAME_SIZE - 1] = '\0';
                break;

            default:
                // Already handled above
                break;
        }

        // Only set success if we didn't encounter any errors
        sat_status_success (&status);

    } while (false);

    return status;
}

void sat_discovery_frame_print (const sat_discovery_frame_t *const object)
{
    if (object == NULL)
    {
        printf ("Invalid frame object\n");
        return;
    }

    printf ("Frame Version: %u\n", object->header.version);
    printf ("Frame Type: %u\n", object->header.type);

    char uuid_string[SAT_UUID_STRING_SIZE];
    sat_uuid_bin_to_string (object->header.uuid, uuid_string, sat_uuid_format_lower_case);
    printf ("UUID: %s\n", uuid_string);

    switch (object->header.type)
    {
        case sat_discovery_frame_type_announce:
            printf ("Service Name: %s\n", object->payload.announce.service_name);
            printf ("Service Port: %u\n", object->payload.announce.service_port);
            printf ("Address: %u\n", object->payload.announce.address);
            break;

        case sat_discovery_frame_type_interest:
            printf ("Service Name: %s\n", object->payload.interest.service_name);
            break;

        case sat_discovery_frame_type_heartbeat:
            printf ("Timestamp: %llu\n", (unsigned long long)object->payload.heartbeat.timestamp);
            break;

        case sat_discovery_frame_type_vanish:
            break;

        default:
            printf ("Unknown frame type\n");
            break;
    }
}