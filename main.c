#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

#include <linux/if.h>

#include <pcap.h>

#include "main.h"

int main(int argc, const char *argv[]) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *pcap;
    radio_header_t *radio_header;
    struct pcap_pkthdr *header;
    const u_char *packet;
    body_t *body;
    mac_t mac;
    int res;
    int offset;
    char signal;

    /*
        args check
    */
    if (argc != 3) {
        ERR("Usage: %s <interface> <mac>", *argv);
    }

    /*
        interface name length check
    */
    if (strnlen(argv[1], IFNAMSIZ) >= IFNAMSIZ) {
        ERR("Invalid interface name!!");
    }

    if (sscanf(argv[2], "%02hhx:%02hhx:%02hhx:%02hhx:%02hhx:%02hhx", MAC_TO_CHAR_P(mac)) != 6) {
        ERR("Invalid mac address!!");
    }

    if (!(pcap = pcap_open_live(argv[1], BUFSIZ, 1, 1000, errbuf))) {
        ERR("Could not open the interface %s!\n%s", argv[1], errbuf);
    }

    while(1) {
        res = pcap_next_ex(pcap, &header, &packet);
        if (res == PCAP_ERROR || res == PCAP_ERROR_BREAK) {
            ERR("Could not read a packet!");
        }

        radio_header = (radio_header_t*)packet;
        body = (body_t*)(packet + radio_header->length);

        if (memcmp(mac, body->trasmitter, 6)) {
            continue;
        }

        if (radio_header->Signal == 0) {
            continue;
        }

        offset = radio_header->TSFT * 8 + \
                radio_header->Flags * 1 + \
                radio_header->Rate * 1 + \
                radio_header->Channel * 4 + \
                radio_header->FHSS * 2 + \
                radio_header->Ext * 4;
        
        signal = radio_header->fields[offset];

        printf("%s\t%hhd\n", argv[2], signal);
    }

}
