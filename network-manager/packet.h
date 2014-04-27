#ifndef __PACKET_H__
#define __PACKET_H___

typedef struct
{
	char src_mac[6];
	char dst_hdr[6];
	unsigned char nxt_layer;
} eth_hdr;

typedef struct
{
	char src_ip[4];
	char dst_ip[4];
	unsigned char nxt_layer;
} ip4_hdr;



typedef struct
{
    eth_hdr eth_hdr;
    ip4_hdr ip_hdr;
} packet;

#endif
