#include <sat_discovery_frame.h>
#include <string.h>
#include <arpa/inet.h>
#include <sat_time.h>
#include <stddef.h>
#include <endian.h>
#include <stdio.h>
#include <sat_log.h>

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
                strncpy (object->payload.announce.service_port, args->service_port, SAT_DISCOVERY_FRAME_PORT - 1);
                object->payload.announce.service_port [SAT_DISCOVERY_FRAME_PORT - 1] = '\0';
                strncpy (object->payload.announce.address, args->address, SAT_DISCOVERY_FRAME_ADDRESS - 1);
                object->payload.announce.address [SAT_DISCOVERY_FRAME_ADDRESS - 1] = '\0';

                break;

            case sat_discovery_frame_type_interest:
            case sat_discovery_frame_type_heartbeat:
            case sat_discovery_frame_type_vanish:
                strncpy (object->payload.interest.service_name, args->service_name, SAT_DISCOVERY_FRAME_SERVICE_NAME_SIZE - 1);
                object->payload.interest.service_name [SAT_DISCOVERY_FRAME_SERVICE_NAME_SIZE - 1] = '\0';
                break;

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

        switch (object->header.type)
        {
            case sat_discovery_frame_type_announce:
                memcpy (&buffer [sizeof (sat_discovery_frame_header_t)], object->payload.announce.service_name, SAT_DISCOVERY_FRAME_SERVICE_NAME_SIZE);
                memcpy (&buffer [sizeof (sat_discovery_frame_header_t) + SAT_DISCOVERY_FRAME_SERVICE_NAME_SIZE], object->payload.announce.service_port, SAT_DISCOVERY_FRAME_PORT);
                memcpy (&buffer [sizeof (sat_discovery_frame_header_t) + SAT_DISCOVERY_FRAME_SERVICE_NAME_SIZE + SAT_DISCOVERY_FRAME_PORT], object->payload.announce.address, SAT_DISCOVERY_FRAME_ADDRESS);
                break;

            case sat_discovery_frame_type_interest:
                memcpy (&buffer [sizeof (sat_discovery_frame_header_t)], object->payload.interest.service_name, SAT_DISCOVERY_FRAME_SERVICE_NAME_SIZE);
                break;

            case sat_discovery_frame_type_heartbeat:
                memcpy (&buffer [sizeof (sat_discovery_frame_header_t)], object->payload.heartbeat.service_name, SAT_DISCOVERY_FRAME_SERVICE_NAME_SIZE);
                break;

            case sat_discovery_frame_type_vanish:
                memcpy (&buffer [sizeof (sat_discovery_frame_header_t)], object->payload.vanish.service_name, SAT_DISCOVERY_FRAME_SERVICE_NAME_SIZE);
                break;

            default:
                // This should never be reached due to validation above
                sat_status_failure (&status, "Invalid frame type");
                break;
        }

        sat_log_debug ("Packed frame contents: %*.s\n", sizeof (sat_discovery_frame_buffer_t), buffer);

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

        // memset (object, 0, sizeof (sat_discovery_frame_t));

        // Copy the frame data from the buffer
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
                strncpy (object->payload.announce.service_name, (const char *)&buffer [18], SAT_DISCOVERY_FRAME_SERVICE_NAME_SIZE - 1);
                memcpy (object->payload.announce.service_port, &buffer [18 + SAT_DISCOVERY_FRAME_SERVICE_NAME_SIZE], SAT_DISCOVERY_FRAME_PORT);
                memcpy (object->payload.announce.address, &buffer [18 + SAT_DISCOVERY_FRAME_SERVICE_NAME_SIZE + SAT_DISCOVERY_FRAME_PORT], SAT_DISCOVERY_FRAME_ADDRESS);
                break;

            case sat_discovery_frame_type_heartbeat:
                strncpy (object->payload.heartbeat.service_name, (const char *)&buffer [18], SAT_DISCOVERY_FRAME_SERVICE_NAME_SIZE - 1);
                break;
            case sat_discovery_frame_type_interest:
                strncpy (object->payload.interest.service_name, (const char *)&buffer [18], SAT_DISCOVERY_FRAME_SERVICE_NAME_SIZE - 1);
                break;
            case sat_discovery_frame_type_vanish:
                // No multi-byte fields to convert
                strncpy (object->payload.vanish.service_name, (const char *)&buffer [18], SAT_DISCOVERY_FRAME_SERVICE_NAME_SIZE - 1);
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
                object->payload.announce.service_name [SAT_DISCOVERY_FRAME_SERVICE_NAME_SIZE - 1] = '\0';
                object->payload.announce.service_port [SAT_DISCOVERY_FRAME_PORT - 1] = '\0';
                object->payload.announce.address [SAT_DISCOVERY_FRAME_ADDRESS - 1] = '\0';
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
        printf ("Frame is NULL\n");
        return;
    }

    printf ("Frame Version: %u\n", object->header.version);
    printf ("Frame Type: %u\n", object->header.type);
    printf ("Frame UUID: ");
    for (size_t i = 0; i < SAT_UUID_BINARY_SIZE; i++)
    {
        printf ("%02X", object->header.uuid[i]);
    }
    printf ("\n");

    switch (object->header.type)
    {
        case sat_discovery_frame_type_announce:
            printf ("Service Name: %s\n", object->payload.announce.service_name);
            printf ("Service Port: %s\n", object->payload.announce.service_port);
            printf ("Address: %s\n", object->payload.announce.address);
            break;

        case sat_discovery_frame_type_interest:
            printf ("Service Name: %s\n", object->payload.interest.service_name);
            break;

        case sat_discovery_frame_type_heartbeat:
            printf ("Service Name: %s\n", object->payload.heartbeat.service_name);
            break;

        case sat_discovery_frame_type_vanish:
            printf ("Service Name: %s\n", object->payload.vanish.service_name);
            break;

        default:
            printf ("Unknown frame type\n");
            break;
    }
}

void sat_discovery_frame_buffer_print (const sat_discovery_frame_buffer_t buffer)
{
    if (buffer == NULL)
    {
        printf ("Buffer is NULL\n");
        return;
    }

    printf ("Frame Buffer: ");
    for (size_t i = 0; i < SAT_DISCOVERY_FRAME_SIZE; i++)
    {
        printf ("%02X ", buffer[i]);
    }
    printf ("\n");
}