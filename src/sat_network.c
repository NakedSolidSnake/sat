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