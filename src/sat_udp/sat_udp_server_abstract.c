#include <sat_udp_server_abstract.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

static void sat_udp_server_abstract_copy_to_context (sat_udp_server_abstract_t *object, sat_udp_server_args_t *args);

static sat_status_t sat_udp_server_abstract_set_socket (sat_udp_server_abstract_t *object, struct addrinfo *info);
static sat_status_t sat_udp_server_abstract_set_reuse_address (sat_udp_server_abstract_t *object);
static sat_status_t sat_udp_server_abstract_set_bind (sat_udp_server_abstract_t *object, struct addrinfo *info);
static sat_status_t sat_udp_server_abstract_is_args_valid (sat_udp_server_args_t *args);
static sat_status_t sat_udp_server_abstract_configure (sat_udp_server_abstract_t *object, struct addrinfo *info_list);
static sat_status_t sat_udp_server_abstract_multicast_enable (sat_udp_server_abstract_t *object, sat_udp_server_args_t *args);

static sat_status_t sat_udp_server_abstract_try_enable_multicast_ipv4 (sat_udp_server_abstract_t *object, const char *address);
static sat_status_t sat_udp_server_abstract_try_enable_multicast_ipv6 (sat_udp_server_abstract_t *object, const char *address);

static struct addrinfo *sat_udp_server_abstract_get_info_list (sat_udp_server_args_t *args);

static void sat_udp_server_abstract_copy_to_context (sat_udp_server_abstract_t *object, sat_udp_server_args_t *args)
{
    object->buffer = args->buffer;
    object->size = args->size;
    object->service = args->service;
    object->events.on_receive = args->events.on_receive;
    object->events.on_send = args->events.on_send;
    object->events.on_multicast_join = args->events.on_multicast_join;
    object->data = args->data;
}

static sat_status_t sat_udp_server_abstract_is_args_valid (sat_udp_server_args_t *args)
{
    sat_status_t status = sat_status_set (&status, false, "sat udp server args error");

    if (args->buffer != NULL &&
        args->size > 0 && 
        args->service != NULL)
    {
        sat_status_set (&status, true, "");
    }

    return status;
}

sat_status_t sat_udp_server_abstract_open (sat_udp_server_abstract_t *object, sat_udp_server_args_t *args)
{
    sat_status_t status;

    do 
    {
        status = sat_udp_server_abstract_is_args_valid (args);
        sat_status_break_on_error (status);

        struct addrinfo *info_list = sat_udp_server_abstract_get_info_list (args);
        if (info_list == NULL)
        {
            sat_status_set (&status, false, "sat udp server abstract open cannot get info list error");
            break;
        }

        status = sat_udp_server_abstract_configure (object, info_list);
        sat_status_break_on_error (status);

        sat_udp_server_abstract_copy_to_context (object, args);

        status = sat_udp_server_abstract_multicast_enable (object, args);
        

    } while (false);

    return status;
}

int sat_udp_server_abstract_get_socket (sat_udp_server_abstract_t *object)
{
    return object->socket;
}

static sat_status_t sat_udp_server_abstract_configure (sat_udp_server_abstract_t *object, struct addrinfo *info_list)
{
    sat_status_t status;

    struct addrinfo *info = NULL;

    for (info = info_list; info != NULL; info = info->ai_next)
    {
        status = sat_udp_server_abstract_set_socket (object, info);
        sat_status_continue_on_error (status);

        status = sat_udp_server_abstract_set_reuse_address (object);
        sat_status_break_on_error (status);

        status = sat_udp_server_abstract_set_bind (object, info);
        if (sat_status_get_result (&status) == false)
        {
            close (object->socket);
            continue;
        }

        break;
    }

    return status;
}

static sat_status_t sat_udp_server_abstract_set_socket (sat_udp_server_abstract_t *object, struct addrinfo *info)
{
    sat_status_t status = sat_status_set (&status, false, "sat udp server abstract set socket error");

    object->socket = socket (info->ai_family, info->ai_socktype, info->ai_protocol);

    if (object->socket >= 0)
        sat_status_set (&status, true, "");

    return status;
}

static sat_status_t sat_udp_server_abstract_set_reuse_address (sat_udp_server_abstract_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat udp server abstract set reuse address error");
    int yes = 1;

    if (setsockopt (object->socket, SOL_SOCKET, SO_REUSEADDR, (void *)&yes, sizeof (yes)) == 0)
        sat_status_set (&status, true, "");

    return status;
}

static sat_status_t sat_udp_server_abstract_set_bind (sat_udp_server_abstract_t *object, struct addrinfo *info)
{
    sat_status_t status = sat_status_set (&status, false, "sat udp server abstract set bind error");

    if (bind (object->socket, info->ai_addr, info->ai_addrlen) == 0)
        sat_status_set (&status, true, "");

    return status;
}

static struct addrinfo *sat_udp_server_abstract_get_info_list (sat_udp_server_args_t *args)
{
    struct addrinfo hints;
    struct addrinfo *info_list = NULL;

    memset(&hints, 0, sizeof (hints));

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;
    hints.ai_protocol = IPPROTO_UDP;

    getaddrinfo (NULL, args->service, &hints, &info_list);

    return info_list;
}


static sat_status_t sat_udp_server_abstract_multicast_enable (sat_udp_server_abstract_t *object, sat_udp_server_args_t *args)
{
    sat_status_t status = sat_status_set (&status, true, "");

    if (args->mode == sat_udp_server_mode_multicast && args->multicast_group != NULL)
    {

        do
        {
            status = sat_udp_server_abstract_try_enable_multicast_ipv4 (object, args->multicast_group);
            if (sat_status_get_result (&status) == true)
            {
                break;
            }

            status = sat_udp_server_abstract_try_enable_multicast_ipv6 (object, args->multicast_group);

        } while (false);
    }

    return status;
}

static sat_status_t sat_udp_server_abstract_try_enable_multicast_ipv4 (sat_udp_server_abstract_t *object, const char *address)
{
    sat_status_t status = sat_status_set (&status, true, "sat udp server abstract is multicast ipv4 error");

    struct in_addr addr4;

    if (inet_pton (AF_INET, address, &addr4) == true)
    {
        uint32_t ip_addr = ntohl (addr4.s_addr);

        if (ip_addr >= 0xE0000000 && ip_addr <= 0xEFFFFFFF) // 224.0.0.0 a 239.255.255.255
        {
            struct ip_mreq mreq;

            sat_status_set (&status, false, "sat udp server abstract multicast enable error");

            mreq.imr_multiaddr.s_addr = addr4.s_addr;
            mreq.imr_interface.s_addr = htonl (INADDR_ANY);

            if (setsockopt (object->socket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&mreq, sizeof (mreq)) == 0)
            {
                if (object->events.on_multicast_join != NULL)
                {
                    object->events.on_multicast_join (object->data);
                }
                sat_status_set (&status, true, "");
            }
        }
    }

    return status;
}

static sat_status_t sat_udp_server_abstract_try_enable_multicast_ipv6 (sat_udp_server_abstract_t *object, const char *address)
{
    sat_status_t status = sat_status_set (&status, true, "sat udp server abstract is multicast ipv6 error");

    struct in6_addr addr6;

    if (inet_pton (AF_INET6, address, &addr6) == true && addr6.s6_addr [0] == 0xFF) // IPv6 starts with FF
    {
        sat_status_set (&status, false, "sat udp server abstract multicast enable error");

        struct ipv6_mreq mreq;

        inet_pton (AF_INET6, address, &mreq.ipv6mr_multiaddr);

        mreq.ipv6mr_interface = 0;

        if (setsockopt (object->socket, IPPROTO_IPV6, IPV6_ADD_MEMBERSHIP, &mreq, sizeof (mreq)) == 0)
        {
            if (object->events.on_multicast_join != NULL)
            {
                object->events.on_multicast_join (object->data);
            }

            sat_status_set (&status, true, "");
        }
    }

    return status;
}