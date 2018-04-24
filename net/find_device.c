/* Compile with: gcc find_device.c -lpcap */
#include <stdio.h>
#include <pcap.h>
#include <arpa/inet.h>
#include <string.h>

int main(int argc, char **argv) {
    // char *device; /* Name of device (e.g. eth0, wlan0) */
    pcap_if_t *interfaces;
    char error_buffer[PCAP_ERRBUF_SIZE]; /* Size defined in pcap.h */

    /* Find a device */
    // device = pcap_lookupdev(error_buffer);
    if (pcap_findalldevs(&interfaces, error_buffer) == -1)
    {
        printf("Error finding devices: %s\n", error_buffer);
        return -1;
    }

    printf("Network devices found:\n");
    int i = 0;    
    for (pcap_if_t *device = interfaces; device != NULL; device = device->next)
    {
            printf("Device: %s\n", device->name);
            for (pcap_addr_t *temp = device->addresses; temp != NULL; temp = temp->next)
            {
                if (temp->addr->sa_family == AF_INET)
                {
                    printf("\tIP: %s\n", inet_ntoa(((struct sockaddr_in*) temp->addr)->sin_addr));
                }
                // else //if (temp->addr->sa_family == AF_)
                // {
                //     printf("\t__: %s\n", inet_ntoa(((struct sockaddr_in*) temp->addr)->sin_addr));
                // }
            }
    }
    pcap_freealldevs(interfaces);
    return 0;
}
