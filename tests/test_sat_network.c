#include <sat_network.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

int main (void)
{
    char ip_address [SAT_NETWORK_IP_MAX_LEN + 1] = {0};

    sat_status_t status = sat_network_get_public_ip (NULL, 0, ip_address);

    assert (sat_status_get_result (&status) == true);

    sat_network_class_t ip_class;

    status = sat_network_get_ip_class (ip_address, &ip_class);
    assert (sat_status_get_result (&status) == true);
    
    printf ("Public IP Address: %s\n", ip_address);
    printf ("IP Address Class: %s\n", sat_network_get_ip_class_string (ip_class));
    
    // Only assert if we got a valid IP address
    if (strlen (ip_address) > 0)
    {
        // Note: The retrieved IP might be private if behind NAT, so we don't assert the class
        printf ("Note: IP classification depends on network configuration\n");
    }

    // Additional IP address classification tests
    printf ("\n=== Additional IP Classification Tests ===\n");
    
    // Test IPv4 and IPv6 addresses
    const char *test_ips[] = 
    {
        // IPv4 addresses
        "192.168.1.1",              // Private Class C
        "10.0.0.1",                 // Private Class A
        "172.16.0.1",               // Private Class B
        "127.0.0.1",                // Loopback
        "8.8.8.8",                  // Public (Google DNS)
        "1.1.1.1",                  // Public (Cloudflare DNS)
        "224.0.0.1",                // Multicast
        "255.255.255.255",          // Broadcast
        
        // IPv6 addresses
        "::1",                      // IPv6 Loopback
        "2001:4860:4860::8888",     // IPv6 Public (Google DNS)
        "2606:4700:4700::1111",     // IPv6 Public (Cloudflare DNS)
        "fe80::1",                  // IPv6 Link-local (private)
        "fc00::1",                  // IPv6 Unique local (private)
        "ff02::1",                  // IPv6 Multicast
        "::ffff:8.8.8.8",          // IPv4-mapped IPv6 (public)
        "::ffff:192.168.1.1"       // IPv4-mapped IPv6 (private)
    };
    
    const char *test_descriptions[] = 
    {
        // IPv4 descriptions
        "IPv4 Private Class C",
        "IPv4 Private Class A", 
        "IPv4 Private Class B",
        "IPv4 Loopback",
        "IPv4 Public (Google DNS)",
        "IPv4 Public (Cloudflare DNS)",
        "IPv4 Multicast",
        "IPv4 Broadcast",
        
        // IPv6 descriptions
        "IPv6 Loopback",
        "IPv6 Public (Google DNS)",
        "IPv6 Public (Cloudflare DNS)",
        "IPv6 Link-local (Private)",
        "IPv6 Unique Local (Private)",
        "IPv6 Multicast",
        "IPv4-mapped IPv6 (Public)",
        "IPv4-mapped IPv6 (Private)"
    };
    
    size_t num_tests = sizeof (test_ips) / sizeof (test_ips[0]);
    
    for (size_t i = 0; i < num_tests; i++)
    {
        sat_network_class_t test_ip_class;
        sat_status_t test_status = sat_network_get_ip_class (test_ips [i], &test_ip_class);
        
        if (sat_status_get_result (&test_status) == true)
        {
            printf ("IP: %-15s (%s) -> %s\n", 
                    test_ips [i], 
                    test_descriptions [i],
                    sat_network_get_ip_class_string (test_ip_class));
        }
        else
        {
            printf ("IP: %-15s (%s) -> Classification failed\n", 
                    test_ips [i], 
                    test_descriptions [i]);
        }
    }
    
    // Test IPv6 DNS connectivity (if available)
    printf ("\n=== Testing IPv6 DNS Connectivity ===\n");
    char ipv6_address [SAT_NETWORK_IP_MAX_LEN + 1] = {0};
    sat_status_t ipv6_status = sat_network_get_public_ip ("2001:4860:4860::8888", 53, ipv6_address);
    
    if (sat_status_get_result (&ipv6_status) == true)
    {
        sat_network_class_t ipv6_class;
        sat_status_t class_status = sat_network_get_ip_class (ipv6_address, &ipv6_class);
        
        if (sat_status_get_result (&class_status) == true)
        {
            printf ("IPv6 Public IP: %s\n", ipv6_address);
            printf ("IPv6 IP Class: %s\n", sat_network_get_ip_class_string (ipv6_class));
        }
    }
    else
    {
        printf ("IPv6 connectivity not available or failed\n");
    }

    return 0;
}