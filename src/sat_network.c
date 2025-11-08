#include <sat_network.h>
#include <sat_iterator.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <ifaddrs.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

#define SAT_NETWORK_DNS_SERVER_DEFAULT "8.8.8.8"
#define SAT_NETWORK_DNS_SERVER_IPV6_DEFAULT "2001:4860:4860::8888"
#define SAT_NETWORK_DNS_PORT_DEFAULT 53


static void sat_network_info_get_ipv4 (sat_network_info_t *info, struct ifaddrs *ifa);
static void sat_network_info_get_ipv6 (sat_network_info_t *info, struct ifaddrs *ifa);

static void sat_network_info_create (sat_network_info_t *info, struct ifaddrs *ifa);

static struct sockaddr_storage sat_network_create_address (const char *const ip_address, uint16_t port)
{
    struct sockaddr_storage address;
    memset (&address, 0, sizeof (address));

    struct sockaddr_in *addr4 = (struct sockaddr_in*)&address;
    struct sockaddr_in6 *addr6 = (struct sockaddr_in6*)&address;

    // Try IPv4 first
    if (inet_pton (AF_INET, ip_address, &addr4->sin_addr) == 1)
    {
        addr4->sin_family = AF_INET;
        addr4->sin_port = htons (port);
    }
    // Try IPv6
    else if (inet_pton (AF_INET6, ip_address, &addr6->sin6_addr) == 1)
    {
        addr6->sin6_family = AF_INET6;
        addr6->sin6_port = htons (port);
    }
    // If neither IPv4 nor IPv6, default to IPv4 (for backward compatibility)
    else
    {
        addr4->sin_family = AF_INET;
        addr4->sin_port = htons (port);
        // Leave address as 0.0.0.0 for invalid input
    }

    return address;
}

static bool sat_network_get_connected_address (int socket, char *ip_address, size_t size)
{
    bool status = false;
    struct sockaddr_storage address;
    socklen_t address_len = sizeof (address);

    if (getsockname (socket, (struct sockaddr*)&address, &address_len) >= 0)
    {
        if (address.ss_family == AF_INET)
        {
            struct sockaddr_in *addr4 = (struct sockaddr_in*)&address;
            inet_ntop (AF_INET, &addr4->sin_addr, ip_address, size);
            status = true;
        }
        else if (address.ss_family == AF_INET6)
        {
            struct sockaddr_in6 *addr6 = (struct sockaddr_in6*)&address;
            inet_ntop (AF_INET6, &addr6->sin6_addr, ip_address, size);
            status = true;
        }
    }

    return status;
}

sat_status_t sat_network_get_ip_class (const char *const ip_address, sat_network_class_t *const ip_class)
{
    sat_status_t status;

    do
    {
        struct in_addr addr4;
        struct in6_addr addr6;

        // Try IPv4 first
        if (inet_pton (AF_INET, ip_address, &addr4) == 1)
        {
            // IPv4 address processing
            uint32_t ip = ntohl (addr4.s_addr);

            // Multicast: 224.0.0.0 to 239.255.255.255 (0xE0000000 to 0xEFFFFFFF)
            if (ip >= 0xE0000000 && ip <= 0xEFFFFFFF)
            {
                *ip_class = sat_network_class_multicast;
            }
            // Broadcast: 255.255.255.255 (0xFFFFFFFF)
            else if (ip == 0xFFFFFFFF)
            {
                *ip_class = sat_network_class_broadcast;
            }
            // Loopback: 127.0.0.0 to 127.255.255.255 (0x7F000000 to 0x7FFFFFFF)
            else if (ip >= 0x7F000000 && ip <= 0x7FFFFFFF)
            {
                *ip_class = sat_network_class_loopback;
            }
            // Private networks (RFC 1918)
            // 10.0.0.0/8: 10.0.0.0 to 10.255.255.255 (0x0A000000 to 0x0AFFFFFF)
            // 172.16.0.0/12: 172.16.0.0 to 172.31.255.255 (0xAC100000 to 0xAC1FFFFF)
            // 192.168.0.0/16: 192.168.0.0 to 192.168.255.255 (0xC0A80000 to 0xC0A8FFFF)
            else if ((ip >= 0x0A000000 && ip <= 0x0AFFFFFF) ||
                     (ip >= 0xAC100000 && ip <= 0xAC1FFFFF) ||
                     (ip >= 0xC0A80000 && ip <= 0xC0A8FFFF))
            {
                *ip_class = sat_network_class_private;
            }
            else
            {
                *ip_class = sat_network_class_public;
            }
        }
        // Try IPv6
        else if (inet_pton (AF_INET6, ip_address, &addr6) == 1)
        {
            // IPv6 address processing
            uint8_t *bytes = addr6.s6_addr;

            // Loopback: ::1
            if (IN6_IS_ADDR_LOOPBACK (&addr6))
            {
                *ip_class = sat_network_class_loopback;
            }
            // Multicast: ff00::/8
            else if (IN6_IS_ADDR_MULTICAST (&addr6))
            {
                *ip_class = sat_network_class_multicast;
            }
            // Link-local: fe80::/10 - considered private
            else if (IN6_IS_ADDR_LINKLOCAL (&addr6))
            {
                *ip_class = sat_network_class_private;
            }
            // Site-local (deprecated): fec0::/10 - considered private
            else if (IN6_IS_ADDR_SITELOCAL (&addr6))
            {
                *ip_class = sat_network_class_private;
            }
            // Unique local addresses: fc00::/7 - private
            else if ((bytes[0] & 0xfe) == 0xfc)
            {
                *ip_class = sat_network_class_private;
            }
            // IPv4-mapped IPv6: ::ffff:0:0/96
            else if (IN6_IS_ADDR_V4MAPPED (&addr6))
            {
                // Extract the IPv4 part and classify it
                uint32_t ipv4_part = ntohl (*((uint32_t*)&bytes[12]));
                
                if (ipv4_part >= 0xE0000000 && ipv4_part <= 0xEFFFFFFF)
                {
                    *ip_class = sat_network_class_multicast;
                }
                else if (ipv4_part == 0xFFFFFFFF)
                {
                    *ip_class = sat_network_class_broadcast;
                }
                else if (ipv4_part >= 0x7F000000 && ipv4_part <= 0x7FFFFFFF)
                {
                    *ip_class = sat_network_class_loopback;
                }
                else if ((ipv4_part >= 0x0A000000 && ipv4_part <= 0x0AFFFFFF) ||
                         (ipv4_part >= 0xAC100000 && ipv4_part <= 0xAC1FFFFF) ||
                         (ipv4_part >= 0xC0A80000 && ipv4_part <= 0xC0A8FFFF))
                {
                    *ip_class = sat_network_class_private;
                }
                else
                {
                    *ip_class = sat_network_class_public;
                }
            }
            else
            {
                // Global unicast addresses - public
                *ip_class = sat_network_class_public;
            }
        }
        else
        {
            sat_status_set (&status, false, "Invalid IP address format");
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
        if (dns_address != NULL && strlen (dns_address) > 0)
        {
            dns = dns_address;
            port = dns_port;
        }

        struct sockaddr_storage address = sat_network_create_address (dns, port);
        
        // Determine socket family based on the address
        int socket_family = AF_INET; // Default to IPv4
        socklen_t addr_len = sizeof (struct sockaddr_in);
        
        if (address.ss_family == AF_INET6)
        {
            socket_family = AF_INET6;
            addr_len = sizeof (struct sockaddr_in6);
        }

        int __socket = socket (socket_family, SOCK_DGRAM, 0);

        if (__socket < 0)
        {
            sat_status_set (&status, false, "Failed to create socket");
            break;
        }

        if (connect (__socket, (struct sockaddr*)&address, addr_len) < 0)
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

sat_status_t sat_network_get_socket_info (int socket, sat_network_socket_info_t *const socket_info)
{
    sat_status_t status;

    do
    {
        if (socket_info == NULL)
        {
            sat_status_set (&status, false, "socket_info is NULL");
            break;
        }

        struct sockaddr_storage address;
        socklen_t address_len = sizeof (address);

        if (getsockname (socket, (struct sockaddr*)&address, &address_len) < 0)
        {
            sat_status_set (&status, false, "Failed to get socket name");
            break;
        }

        if (address.ss_family == AF_INET)
        {
            struct sockaddr_in *addr4 = (struct sockaddr_in*)&address;
            inet_ntop (AF_INET, &addr4->sin_addr, socket_info->address, SAT_NETWORK_ADDRESS_SIZE + 1);
            socket_info->port = ntohs (addr4->sin_port);
        }
        else if (address.ss_family == AF_INET6)
        {
            struct sockaddr_in6 *addr6 = (struct sockaddr_in6*)&address;
            inet_ntop (AF_INET6, &addr6->sin6_addr, socket_info->address, SAT_NETWORK_ADDRESS_SIZE + 1);
            socket_info->port = ntohs (addr6->sin6_port);
        }
        else
        {
            sat_status_set (&status, false, "Unknown address family");
            break;
        }

        sat_status_set (&status, true, "");

    } while (false);

    return status;
}

sat_status_t sat_network_get_info_by_interface (sat_network_info_t *const info, const char *const interface_name)
{
    sat_status_t status;
    struct ifaddrs *ifaddr;
    struct ifaddrs *ifa;

    do 
    {
        if (getifaddrs (&ifaddr) == -1)
        {
            sat_status_failure (&status, "getifaddrs failed");
            break;
        }

        for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next)
        {
            if (ifa->ifa_addr == NULL ||
                (ifa->ifa_addr->sa_family != AF_INET &&
                ifa->ifa_addr->sa_family != AF_INET6) || 
                strcmp(ifa->ifa_name, interface_name) != 0)
            {
                continue;
            }

            sat_network_info_create (info, ifa);

            sat_status_success (&status);

            break;
        }

        freeifaddrs (ifaddr);

    } while (false);

    return status;
}

sat_status_t sat_network_get_info_list (sat_array_t **array)
{
    sat_status_t status;
    struct ifaddrs *ifaddr;
    struct ifaddrs *ifa;
    uint16_t index = 0;
    sat_network_info_t info;

    do 
    {
        status = sat_array_create (array, &(sat_array_args_t)
                                    {
                                        .size = 1,
                                        .object_size = sizeof (sat_network_info_t),
                                        .mode = sat_array_mode_dynamic
                                    });

        if (sat_status_get_result (&status) == false)
        {
            break;
        }

        if (getifaddrs (&ifaddr) == -1)
        {
            sat_status_failure (&status, "getifaddrs failed");
            break;
        }

        for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next)
        {
            if (ifa->ifa_addr == NULL ||
                (ifa->ifa_addr->sa_family != AF_INET &&
                ifa->ifa_addr->sa_family != AF_INET6))
            {
                continue;
            }

            sat_network_info_create (&info, ifa);

            sat_array_add (*array, &info);

            index ++;
        }

        freeifaddrs (ifaddr);

        sat_status_success (&status);

    } while (false);

    return status;
}

void sat_network_socket_info_debug (const sat_network_socket_info_t *const socket_info)
{
    if (socket_info != NULL)
    {
        printf ("Socket Info:\n");
        printf ("  Address: %s\n", socket_info->address);
        printf ("  Port   : %u\n", socket_info->port);
    }
}

void sat_network_info_debug (const sat_network_info_t *const network_info)
{
    if (network_info != NULL)
    {
        printf ("Network Info:\n");
        printf ("  Interface Name: %s\n", network_info->interface_name);
        printf ("  IP Address    : %s\n", network_info->ip_address);
        printf ("  Netmask       : %s\n", network_info->netmask);
    }
}

void sat_network_info_list_debug (sat_array_t *info)
{
    if (info != NULL)
    {
        sat_iterator_t it;
        sat_iterator_open (&it, (sat_iterator_base_t *)info);

        sat_network_info_t *network_info = sat_iterator_next (&it);
        while (network_info != NULL)
        {

            printf ("Network Info:\n");
            printf ("  Interface Name: %s\n", network_info->interface_name);
            printf ("  IP Address    : %s\n", network_info->ip_address);
            printf ("  Netmask       : %s\n", network_info->netmask);
            printf ("  Family        : %s\n", network_info->family == sat_network_family_ipv4 ? "IPv4" : "IPv6");

            network_info = sat_iterator_next (&it);
        }
    }
}


static void sat_network_info_get_ipv4 (sat_network_info_t *info, struct ifaddrs *ifa)
{
    struct sockaddr_in *addr_in = (struct sockaddr_in *)ifa->ifa_addr;
    
    inet_ntop (AF_INET, &addr_in->sin_addr, info->ip_address, sizeof (info->ip_address));
    
    if (ifa->ifa_netmask != NULL)
    {
        struct sockaddr_in *mask_in = (struct sockaddr_in *)ifa->ifa_netmask;

        inet_ntop (AF_INET, &mask_in->sin_addr, info->netmask, sizeof (info->netmask));
    }
}

static void sat_network_info_get_ipv6 (sat_network_info_t *info, struct ifaddrs *ifa)
{
    struct sockaddr_in6 *addr_in6 = (struct sockaddr_in6 *)ifa->ifa_addr;

    inet_ntop (AF_INET6, &addr_in6->sin6_addr, info->ip_address, sizeof (info->ip_address));

    if (ifa->ifa_netmask != NULL)
    {
        struct sockaddr_in6 *mask_in6 = (struct sockaddr_in6 *)ifa->ifa_netmask;

        inet_ntop (AF_INET6, &mask_in6->sin6_addr, info->netmask, sizeof (info->netmask));
    }
}

static void sat_network_info_create (sat_network_info_t *info, struct ifaddrs *ifa)
{
    memset (info, 0, sizeof (sat_network_info_t));

    strncpy (info->interface_name, ifa->ifa_name, SAT_NETWORK_INTERFACE_NAME_SIZE);

    if (ifa->ifa_addr->sa_family == AF_INET)
    {
        info->family = sat_network_family_ipv4;
        sat_network_info_get_ipv4 (info, ifa);
    }
    else if (ifa->ifa_addr->sa_family == AF_INET6)
    {
        info->family = sat_network_family_ipv6;
        sat_network_info_get_ipv6 (info, ifa);
    }
}