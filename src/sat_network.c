#include <sat_network.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <ifaddrs.h>
#include <string.h>
#include <unistd.h>

#define SAT_NETWORK_DNS_SERVER_DEFAULT "8.8.8.8"
#define SAT_NETWORK_DNS_PORT_DEFAULT 53

static struct sockaddr_in sat_network_create_address (const char *const ip_address, uint16_t port)
{
    struct sockaddr_in address;

    memset (&address, 0, sizeof(address));

    address.sin_family = AF_INET;
    address.sin_port = htons (port);

    inet_pton (AF_INET, ip_address, &address.sin_addr);

    return address;
}

static bool sat_network_get_connected_address (int socket, char *ip_address, size_t size)
{
    bool status = false;
    struct sockaddr_in address;
    socklen_t address_len = sizeof (address);

    if (getsockname (socket, (struct sockaddr*)&address, &address_len) >= 0)
    {
        inet_ntop (AF_INET, &address.sin_addr, ip_address, size);

        status = true;
    }

    return status;
}

sat_status_t sat_network_get_ip_class (const char *const ip_address, sat_network_class_t *const ip_class)
{
    sat_status_t status;

    do
    {
        struct in_addr addr;

        if (inet_pton (AF_INET, ip_address, &addr) != 1)
        {
            sat_status_set (&status, false, "Invalid IP address format");
            break;
        }

        uint32_t ip = ntohl (addr.s_addr);

        if ((ip >= 0xE0000000 && ip <= 0xEFFFFFFF))
        {
            *ip_class = sat_network_class_multicast;
        }
        else if ((ip >= 0xFF000000 && ip <= 0xFFFFFFFF))
        {
            *ip_class = sat_network_class_broadcast;
        }
        else if ((ip >= 0x00000000 && ip <= 0x7FFFFFFF))
        {
            *ip_class = sat_network_class_public;
        }
        else if ((ip >= 0x80000000 && ip <= 0xBFFFFFFF))
        {
            *ip_class = sat_network_class_private;
        }
        else if ((ip >= 0x7F000000 && ip <= 0x7FFFFFFF))
        {
            *ip_class = sat_network_class_loopback;
        }
        else
        {
            sat_status_set (&status, false, "Unknown IP address class");
            break;
        }

        sat_status_set (&status, true, "");

    } while (false);

    return status;
}

const char *const sat_network_get_ip_class_string (const sat_network_class_t ip_class)
{
    switch (ip_class)
    {
        case sat_network_class_multicast:
            return "multicast";

        case sat_network_class_broadcast:
            return "broadcast";

        case sat_network_class_public:
            return "public";

        case sat_network_class_private:
            return "private";

        case sat_network_class_loopback:
            return "loopback";
            
        default:
            return "unknown";
    }
}

sat_status_t sat_network_get_public_ip (const char *const dns_address, uint16_t dns_port, char ip_address [SAT_NETWORK_IP_MAX_LEN + 1])
{
    sat_status_t status;
    const char *dns = SAT_NETWORK_DNS_SERVER_DEFAULT;
    uint16_t port = SAT_NETWORK_DNS_PORT_DEFAULT;

    do
    {
        int __socket = socket (AF_INET, SOCK_DGRAM, 0);

        if (__socket < 0)
        {
            sat_status_set (&status, false, "Failed to create socket");
            break;
        }

        if (dns_address != NULL && strlen (dns_address) > 0)
        {
            dns = dns_address;
            port = dns_port;
        }

        struct sockaddr_in address = sat_network_create_address (dns, port);

        if (connect (__socket, (struct sockaddr*)&address, sizeof (address)) < 0)
        {
            close (__socket);
            sat_status_set (&status, false, "Failed to connect");
            break;
        }

        if (sat_network_get_connected_address (__socket, ip_address, SAT_NETWORK_IP_MAX_LEN) == false)
        {
            close (__socket);
            sat_status_set (&status, false, "Failed to get connected address");
            break;
        }

        close (__socket);
        sat_status_set (&status, true, "");

    } while (false);

    return status;
}