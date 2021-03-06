#ifndef PKT_H
#define PKT_H 1

#include <stdint.h>

#ifndef __MINGW32__
#define __PTATTR__ __attribute__ ((packed))
#else
#define __PTATTR__ __attribute__ ((packed, gcc_struct))
#endif

#ifdef WIN32
#include <winsock2.h>
typedef int socklen_t;
typedef uint32_t in_addr_t;
#define ETH_ALEN 6 /* Octets in one ethernet addr   */
struct ether_header {
	uint8_t  ether_dhost[ETH_ALEN]; /* destination eth addr */
	uint8_t  ether_shost[ETH_ALEN]; /* source ether addr    */
	uint16_t ether_type;            /* packet type ID field */
} __PTATTR__;
#endif /* WIN32 */

/** Resend packets after this interval (in seconds) */
#define kResend_interval 1.5

/** ping_tunnel_pkt_t: This data structure represents the header of a ptunnel
 * packet, consisting of a magic number, the tunnel's destination IP and port,
 * as well as some other fields. Note that the dest IP and port is only valid
 * in packets from the client to the proxy.
 */
typedef struct {
	/** magic number, used to identify ptunnel packets. */
	uint32_t magic;
	/** destination IP and port (used by proxy to figure */
	uint32_t dst_ip;
	/** out where to tunnel to) */
	uint32_t dst_port;
	/** current connection state; see constants above. */
	uint32_t state;
	/** sequence number of last packet received from other end */
	uint32_t ack;
	/** length of data buffer */
	uint32_t data_len;
	/** sequence number of this packet */
	uint16_t seq_no;
	/** id number, used to separate different tunnels from each other */
	uint16_t id_no;
	/** optional data buffer */
	char data[0];
} __PTATTR__ ping_tunnel_pkt_t;

/** ip_packet_t: This is basically my own definition of the IP packet, which
 * of course complies with the official definition ;) See any good book on IP
 * (or even the RFC) for info on the contents of this packet.
 */
typedef struct {
	uint8_t vers_ihl;
	uint8_t tos;
	uint16_t pkt_len;
	uint16_t id;
	uint16_t flags_frag_offset;
	uint8_t ttl;
	uint8_t proto; // 1 for ICMP
	uint16_t checksum;
	uint32_t src_ip;
	uint32_t dst_ip;
	char data[0];
} __PTATTR__ ip_packet_t;

/** icmp_echo_packet_t: This is the definition of a standard ICMP header. The
 * ptunnel packets are constructed as follows:
 * [    ip header (20 bytes)   ]
 * [   icmp header (8 bytes)   ]
 * [ ptunnel header (28 bytes) ]
 *
 * We actually only create the ICMP and ptunnel headers, the IP header is
 * taken care of by the OS.
 */
typedef struct {
	uint8_t type;
	uint8_t code;
	uint16_t checksum;
	uint16_t identifier;
	uint16_t seq;
	char data[0];
} __PTATTR__ icmp_echo_packet_t;

typedef struct forward_desc_t forward_desc_t;
typedef struct icmp_desc_t icmp_desc_t;


void handle_packet(char *buf, unsigned bytes, int is_pcap, struct sockaddr_in *addr, int icmp_sock);

void handle_data(icmp_echo_packet_t *pkt, int total_len, forward_desc_t **ring,
                 int *await_send, int *insert_idx,  uint16_t *next_expected_seq);

void handle_ack(uint16_t seq_no, icmp_desc_t *ring, int *packets_awaiting_ack,
                int one_ack_only, int insert_idx, int *first_ack,
                uint16_t *remote_ack, int is_pcap);

#endif
