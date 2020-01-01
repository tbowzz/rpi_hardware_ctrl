#include <iostream>
#include <pcap.h>
#include <string>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/if_ether.h>

#define PKTDEF_STATUS        0x01
#define PKTDEF_GPS           0x02
#define PKTDEF_CTRL          0x03
#define PKTDEF_CNFG          0x04
#define PKTDEF_ROUTE         0x05
#define PKTDEF_ACTION        0x06 // TODO
#define PKTDEF_ALIVE         0x07
#define PKTDEF_ARMED         0x08 // TODO

/* default snap length (maximum bytes per packet to capture) */
#define SNAP_LEN 1518

/* ethernet headers are always exactly 14 bytes [1] */
#define SIZE_ETHERNET 14

#define SIZE_UDP 8

/* Ethernet header */
struct sniff_ethernet {
        u_char  ether_dhost[ETHER_ADDR_LEN];    /* destination host address */
        u_char  ether_shost[ETHER_ADDR_LEN];    /* source host address */
        u_short ether_type;                     /* IP? ARP? RARP? etc */
};

/* IP header */
struct sniff_ip {
        u_char  ip_vhl;                 /* version << 4 | header length >> 2 */
        u_char  ip_tos;                 /* type of service */
        u_short ip_len;                 /* total length */
        u_short ip_id;                  /* identification */
        u_short ip_off;                 /* fragment offset field */
        #define IP_RF 0x8000            /* reserved fragment flag */
        #define IP_DF 0x4000            /* dont fragment flag */
        #define IP_MF 0x2000            /* more fragments flag */
        #define IP_OFFMASK 0x1fff       /* mask for fragmenting bits */
        u_char  ip_ttl;                 /* time to live */
        u_char  ip_p;                   /* protocol */
        u_short ip_sum;                 /* checksum */
        struct  in_addr ip_src,ip_dst;  /* source and dest address */
};
#define IP_HL(ip)               (((ip)->ip_vhl) & 0x0f)
#define IP_V(ip)                (((ip)->ip_vhl) >> 4)

struct sniff_udp {
    uint16_t        uh_sport;      /* source port */
    uint16_t        uh_dport;      /* destination port */
    uint16_t        uh_ulen;
    uint16_t        udp_sum;    /* checksum */
};

/* TCP header */
typedef u_int tcp_seq;

struct sniff_tcp {
        u_short th_sport;               /* source port */
        u_short th_dport;               /* destination port */
        tcp_seq th_seq;                 /* sequence number */
        tcp_seq th_ack;                 /* acknowledgement number */
        u_char  th_offx2;               /* data offset, rsvd */
#define TH_OFF(th)      (((th)->th_offx2 & 0xf0) >> 4)
        u_char  th_flags;
        #define TH_FIN  0x01
        #define TH_SYN  0x02
        #define TH_RST  0x04
        #define TH_PUSH 0x08
        #define TH_ACK  0x10
        #define TH_URG  0x20
        #define TH_ECE  0x40
        #define TH_CWR  0x80
        #define TH_FLAGS        (TH_FIN|TH_SYN|TH_RST|TH_ACK|TH_URG|TH_ECE|TH_CWR)
        u_short th_win;                 /* window */
        u_short th_sum;                 /* checksum */
        u_short th_urp;                 /* urgent pointer */
};

#pragma pack(push,1)
struct PacketHeader
{
	uint8_t definition;
	uint8_t packetType;
	uint16_t payloadLength;
	uint16_t sequenceNumber;
	uint32_t destIpAddr;
	uint32_t reserved0;
	uint32_t reserved1;
	// size = 18 bytes
};
#pragma pack(pop)

#define PKT_HEADER_SIZE sizeof(PacketHeader)

void another_callback(u_char *arg, const struct pcap_pkthdr* pkthdr, const u_char* packet)
{
    int i=0;
    static int count=0;

    if (pkthdr->len >= PKT_HEADER_SIZE && pkthdr->len <= 240)
    {
        // printf("Packet Count: %d\n", ++count);    /* Number of Packets */
        printf("Received Packet Size: %d\n", pkthdr->len);    /* Length of header */

        PacketHeader header;
        memcpy(&header, &packet, PKT_HEADER_SIZE);

        printf("header.definition: %d\n", header.definition);
        printf("header.packetType: %d\n", header.packetType);
        printf("header.payloadLength: %d\n", header.payloadLength);
    }
    // printf("Payload:\n");                     /* And now the data */
    // for(i=0;i<pkthdr->len;i++) {
    //     if(isprint(packet[i]))                /* Check if the packet data is printable */
    //         printf("%c ",packet[i]);          /* Print it */
    //     else
    //         printf(" . ",packet[i]);          /* If not print a . */
    //     if((i%16==0 && i!=0) || i==pkthdr->len-1)
    //         printf("\n");
    // }
}

void handle_packet(u_char *arg, const struct pcap_pkthdr* pkthdr, const u_char* packet)
{
    	static int count = 1;                   /* packet counter */
	
	/* declare pointers to packet headers */
	const struct sniff_ethernet *ethernet;  /* The ethernet header [1] */
	const struct sniff_ip *ip;              /* The IP header */
	const struct sniff_udp *udp;            /* The UDP header */
	const char *payload;                    /* Packet payload */

	int size_ip;
	int size_payload;
	
	/* define ethernet header */
	ethernet = (struct sniff_ethernet*)(packet);
	
	/* define/compute ip header offset */
	ip = (struct sniff_ip*)(packet + SIZE_ETHERNET);
	size_ip = IP_HL(ip)*4;
	if (size_ip < 20) {
		printf("   * Invalid IP header length: %u bytes\n", size_ip);
		return;
	}
	
	/* determine protocol */	
	switch(ip->ip_p) {
		case IPPROTO_TCP:
			// printf("   Protocol: TCP\n");
			return;
		case IPPROTO_UDP:
			// printf("   Protocol: UDP\n");
			break;
		case IPPROTO_ICMP:
			printf("   Protocol: ICMP\n");
			return;
		case IPPROTO_IP:
			printf("   Protocol: IP\n");
			return;
		default:
			printf("   Protocol: unknown\n");
			return;
	}

    /*
	 *  OK, this packet is UDP.
	 */

    /* define/compute tcp header offset */
	udp = (struct sniff_udp*)(packet + SIZE_ETHERNET + SIZE_UDP);
    int destPort = ntohs(udp->uh_dport);
    // if (destPort != 24902)
    // {
    //     return;
    // }

    std::string srcIp(inet_ntoa(ip->ip_src));
    std::string destIp(inet_ntoa(ip->ip_dst));
    if (srcIp.compare("10.0.0.54") != 0 || destIp.compare("10.0.0.167"))
    {
        return;
    }

    printf("\nPacket number %d:\n", count);
	count++;
    /* print source and destination IP addresses */
	printf("       From: %s\n", inet_ntoa(ip->ip_src));
	printf("         To: %s\n", inet_ntoa(ip->ip_dst));
	printf("   Src port: %d\n", ntohs(udp->uh_sport));
	printf("   Dst port: %d\n", ntohs(udp->uh_dport));
	
	/* define/compute udp payload (segment) offset */
	payload = (const char *)(packet + SIZE_ETHERNET + size_ip + SIZE_UDP);
	
	/* compute udp payload (segment) size */
	size_payload = ntohs(ip->ip_len) - (size_ip + SIZE_UDP);
    if (size_payload > ntohs(udp->uh_ulen))
    {
        size_payload = ntohs(udp->uh_ulen);
    }
    printf("   Packet size: %d\n", size_payload);


    if (size_payload >= PKT_HEADER_SIZE && size_payload <= 240)
    {
        PacketHeader header;
        memcpy(&header, payload, PKT_HEADER_SIZE);

        if (header.definition <= 0x08)
        {
            printf("   header.definition: %d\n", header.definition);
            printf("   header.packetType: %d\n", header.packetType);
            printf("   header.payloadLength: %d\n", header.payloadLength);
        }
    }
	
	/*
	 * Print payload data; it might be binary, so don't just
	 * treat it as a string.
	 */
	// if (size_payload > 0) {
	// 	printf("   Payload (%d bytes):\n", size_payload);
	// 	print_payload(payload, size_payload);
	// }

	return;
}

int main(int argc,char **argv)
{
    char *dev;
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t* descr;
    const u_char *packet;
    struct pcap_pkthdr hdr;
    struct ether_header *eptr;    /* net/ethernet.h */
    struct bpf_program fp;        /* hold compiled program */
    bpf_u_int32 maskp;            /* subnet mask */
    bpf_u_int32 netp;             /* ip */

    if(argc != 2){
        fprintf(stdout, "Usage: %s \"expression\"\n"
            ,argv[0]);
        return 0;
    }

    /* Now get a device */
    pcap_if_t *interfaces, *temp;
    int i = 0;
    if(pcap_findalldevs(&interfaces, errbuf) == -1)
    {
        printf("error in pcap findall devs\n");
        return -1;   
    }

    printf("The interfaces present on the system are:\n");
    std::string name;
    for(temp=interfaces;temp;temp=temp->next)
    {
        printf("%d  :  %s\n", i++, temp->name);
        std::string tmpName(temp->name);
        name = tmpName;
        if (name.compare("eth0") == 0)
        {
            printf("Selected %s\n", temp->name);
            break;
        }
    }
    // dev = pcap_lookupdev(errbuf);

    if(!name.size())
    {
        fprintf(stderr, "%s\n", errbuf);
        exit(1);
    }
    printf("Device: %s\n", name.c_str());
        /* Get the network address and mask */
    pcap_lookupnet(name.c_str(), &netp, &maskp, errbuf);

    /* open device for reading in promiscuous mode */
    descr = pcap_open_live(name.c_str(), BUFSIZ, 1,-1, errbuf);
    printf("descr: %s\n", descr);
    if(descr == NULL)
    {
        printf("pcap_open_live(): %s\n", errbuf);
        exit(1);
    }

    /* Now we'll compile the filter expression*/
    if(pcap_compile(descr, &fp, argv[1], 0, netp) == -1) {
        fprintf(stderr, "Error calling pcap_compile\n");
        exit(1);
    }

    /* set the filter */
    if(pcap_setfilter(descr, &fp) == -1) {
        fprintf(stderr, "Error setting filter\n");
        exit(1);
    }

    /* loop for callback function */
    pcap_loop(descr, -1, handle_packet, NULL);
    return 0;
}
