#include <sat_discovery_frame.h>
#include <string.h>
#include <arpa/inet.h>

sat_status_t sat_discover_frame_create (sat_discovery_frame_t *const object, sat_discovery_frame_args_t *args)
{
    sat_status_t status;

    do
    {
        // Validate input parameters
        if (object == NULL || args == NULL)
        {
            sat_status_set (&status, false, "Invalid parameters");
            break;
        }

        // Clear the object memory
        memset (object, 0, sizeof (sat_discovery_frame_t));

        // Copy values from args to object
        object->version = args->version;
        object->type = args->type;
        object->flags = args->flags;
        object->timestamp = args->timestamp;
        object->service_port = args->service_port;
        object->address = args->address;

        // Copy service name with bounds checking
        strncpy (object->service_name, args->service_name, SAT_DISCOVERY_FRAME_SERVICE_NAME_SIZE - 1);
        object->service_name [SAT_DISCOVERY_FRAME_SERVICE_NAME_SIZE - 1] = '\0';

        sat_status_set (&status, true, "");

    } while (false);

    return status;
}

sat_status_t sat_discover_frame_pack (const sat_discovery_frame_t *const object, sat_discovery_frame_buffer_t *const buffer)
{
    sat_status_t status;

    do
    {
        // Validate input parameters
        if (object == NULL || buffer == NULL)
        {
            sat_status_set (&status, false, "Invalid parameters");
            break;
        }

        // Clear the buffer
        memset (buffer, 0, sizeof (sat_discovery_frame_buffer_t));

        // Create a temporary packed structure for network byte order conversion
        sat_discovery_frame_t packed_frame;
        memcpy (&packed_frame, object, sizeof (sat_discovery_frame_t));

        // Convert multi-byte fields to network byte order
        packed_frame.flags = htonl (packed_frame.flags);
        packed_frame.timestamp = htonl (packed_frame.timestamp);
        packed_frame.service_port = htons (packed_frame.service_port);
        packed_frame.address = htonl (packed_frame.address);

        // Copy the packed structure to the buffer
        memcpy (buffer->buffer, &packed_frame, sizeof (sat_discovery_frame_t));
        buffer->size = sizeof (sat_discovery_frame_t);

        sat_status_set (&status, true, "");

    } while (false);

    return status;
}

sat_status_t sat_discover_frame_unpack (sat_discovery_frame_t *const object, const sat_discovery_frame_buffer_t *const buffer)
{
    sat_status_t status;

    do
    {
        // Validate input parameters
        if (object == NULL || buffer == NULL)
        {
            sat_status_set (&status, false, "Invalid parameters");
            break;
        }

        // Validate buffer size
        if (buffer->size != sizeof (sat_discovery_frame_t))
        {
            sat_status_set (&status, false, "Invalid buffer size");
            break;
        }

        // Copy the buffer data to the object
        memcpy (object, buffer->buffer, sizeof (sat_discovery_frame_t));

        // Convert multi-byte fields from network byte order to host byte order
        object->flags = ntohl (object->flags);
        object->timestamp = ntohl (object->timestamp);
        object->service_port = ntohs (object->service_port);
        object->address = ntohl (object->address);

        // Ensure service name is null-terminated
        object->service_name [SAT_DISCOVERY_FRAME_SERVICE_NAME_SIZE - 1] = '\0';

        sat_status_set (&status, true, "");

    } while (false);

    return status;
}