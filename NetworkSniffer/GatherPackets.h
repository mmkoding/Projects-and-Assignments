#pragma once
#include "NSVarsAndStructs.h"

class Packets {

public:
	virtual void printPacket(u_char* Buffer, int Size) {};

	/*
		Print the IP header for IP packets
	*/
	void PrintIpHeader(unsigned char* Buffer, int Size)
	{
		int iphdrlen = 0;

		iphdr = (IPV4_HDR*)(Buffer + sizeof(ETHER_HDR));
		iphdrlen = iphdr->ip_header_len * 4;

		memset(&source, 0, sizeof(source));
		source.sin_addr.s_addr = iphdr->ip_srcaddr;

		memset(&dest, 0, sizeof(dest));
		dest.sin_addr.s_addr = iphdr->ip_destaddr;

		print_ethernet_header(Buffer);

		fprintf(logfile, "\n");
		fprintf(logfile, "IP Header\n");
		fprintf(logfile, " |-IP Version : %d\n", (unsigned int)iphdr->ip_version);
		fprintf(logfile, " |-IP Header Length : %d DWORDS or %d Bytes\n", (unsigned int)iphdr->ip_header_len, ((unsigned int)(iphdr->ip_header_len)) * 4);
		fprintf(logfile, " |-Type Of Service : %d\n", (unsigned int)iphdr->ip_tos);
		fprintf(logfile, " |-IP Total Length : %d Bytes(Size of Packet)\n", ntohs(iphdr->ip_total_length));
		fprintf(logfile, " |-Identification : %d\n", ntohs(iphdr->ip_id));
		fprintf(logfile, " |-Reserved ZERO Field : %d\n", (unsigned int)iphdr->ip_reserved_zero);
		fprintf(logfile, " |-Dont Fragment Field : %d\n", (unsigned int)iphdr->ip_dont_fragment);
		fprintf(logfile, " |-More Fragment Field : %d\n", (unsigned int)iphdr->ip_more_fragment);
		fprintf(logfile, " |-TTL : %d\n", (unsigned int)iphdr->ip_ttl);
		fprintf(logfile, " |-Protocol : %d\n", (unsigned int)iphdr->ip_protocol);
		fprintf(logfile, " |-Checksum : %d\n", ntohs(iphdr->ip_checksum));
		fprintf(logfile, " |-Source IP : %s\n", inet_ntoa(source.sin_addr));
		fprintf(logfile, " |-Destination IP : %s\n", inet_ntoa(dest.sin_addr));
	}

	/*
		Print the hex values of the data
	*/
	void PrintData(u_char* data, int Size)
	{
		unsigned char a, line[17], c;
		int j;

		//loop over each character and print
		for (i = 0; i < Size; i++)
		{
			c = data[i];

			//Print the hex value for every character , with a space
			fprintf(logfile, " %.2x", (unsigned int)c);

			//Add the character to data line
			a = (c >= 32 && c <= 128) ? (unsigned char)c : '.';

			line[i % 16] = a;

			//if last character of a line , then print the line - 16 characters in 1 line
			if ((i != 0 && (i + 1) % 16 == 0) || i == Size - 1)
			{
				line[i % 16 + 1] = '\0';

				//print a big gap of 10 characters between hex and characters
				fprintf(logfile, "          ");

				//Print additional spaces for last lines which might be less than 16 characters in length
				for (j = strlen((char*)line); j < 16; j++)
				{
					fprintf(logfile, "   ");
				}

				fprintf(logfile, "%s \n", line);
			}
		}

		fprintf(logfile, "\n");
	}

	/*
		Print the Ethernet header
	*/
	void print_ethernet_header(u_char* buffer)
	{
		ETHER_HDR* eth = (ETHER_HDR*)buffer;

		fprintf(logfile, "\n");
		fprintf(logfile, "Ethernet Header\n");
		fprintf(logfile, " |-Destination Address : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X \n", eth->dest[0], eth->dest[1], eth->dest[2], eth->dest[3], eth->dest[4], eth->dest[5]);
		fprintf(logfile, " |-Source Address      : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X \n", eth->source[0], eth->source[1], eth->source[2], eth->source[3], eth->source[4], eth->source[5]);
		fprintf(logfile, " |-Protocol            : 0x%.4x \n", ntohs(eth->type));
	}

};

class TCP : public Packets {
public:
	void printPacket(u_char* Buffer, int Size) {
		unsigned short iphdrlen;
		int header_size = 0, tcphdrlen, data_size;

		iphdr = (IPV4_HDR*)(Buffer + sizeof(ETHER_HDR));
		iphdrlen = iphdr->ip_header_len * 4;

		tcpheader = (TCP_HDR*)(Buffer + iphdrlen + sizeof(ETHER_HDR));
		tcphdrlen = tcpheader->data_offset * 4;

		data = (Buffer + sizeof(ETHER_HDR) + iphdrlen + tcphdrlen);
		data_size = (Size - sizeof(ETHER_HDR) - iphdrlen - tcphdrlen);

		fprintf(logfile, "\n\n***********************TCP Packet*************************\n");

		PrintIpHeader(Buffer, Size);

		fprintf(logfile, "\n");
		fprintf(logfile, "TCP Header\n");
		fprintf(logfile, " |-Source Port : %u\n", ntohs(tcpheader->source_port));
		fprintf(logfile, " |-Destination Port : %u\n", ntohs(tcpheader->dest_port));
		fprintf(logfile, " |-Sequence Number : %u\n", ntohl(tcpheader->sequence));
		fprintf(logfile, " |-Acknowledge Number : %u\n", ntohl(tcpheader->acknowledge));
		fprintf(logfile, " |-Header Length : %d DWORDS or %d BYTES\n", (unsigned int)tcpheader->data_offset, (unsigned int)tcpheader->data_offset * 4);
		fprintf(logfile, " |-CWR Flag : %d\n", (unsigned int)tcpheader->cwr);
		fprintf(logfile, " |-ECN Flag : %d\n", (unsigned int)tcpheader->ecn);
		fprintf(logfile, " |-Urgent Flag : %d\n", (unsigned int)tcpheader->urg);
		fprintf(logfile, " |-Acknowledgement Flag : %d\n", (unsigned int)tcpheader->ack);
		fprintf(logfile, " |-Push Flag : %d\n", (unsigned int)tcpheader->psh);
		fprintf(logfile, " |-Reset Flag : %d\n", (unsigned int)tcpheader->rst);
		fprintf(logfile, " |-Synchronise Flag : %d\n", (unsigned int)tcpheader->syn);
		fprintf(logfile, " |-Finish Flag : %d\n", (unsigned int)tcpheader->fin);
		fprintf(logfile, " |-Window : %d\n", ntohs(tcpheader->window));
		fprintf(logfile, " |-Checksum : %d\n", ntohs(tcpheader->checksum));
		fprintf(logfile, " |-Urgent Pointer : %d\n", tcpheader->urgent_pointer);
		fprintf(logfile, "\n");
		fprintf(logfile, " DATA Dump ");
		fprintf(logfile, "\n");

		fprintf(logfile, "IP Header\n");
		PrintData((u_char*)iphdr, iphdrlen);

		fprintf(logfile, "TCP Header\n");
		PrintData((u_char*)tcpheader, tcphdrlen);

		fprintf(logfile, "Data Payload\n");
		PrintData(data, data_size);

		fprintf(logfile, "\n###########################################################\n");
	}
};

class UDP : public Packets {
public:
	void printPacket(u_char* Buffer, int Size) {
		int iphdrlen = 0, data_size = 0;

		iphdr = (IPV4_HDR*)(Buffer + sizeof(ETHER_HDR));
		iphdrlen = iphdr->ip_header_len * 4;

		udpheader = (UDP_HDR*)(Buffer + iphdrlen + sizeof(ETHER_HDR));

		data = (Buffer + sizeof(ETHER_HDR) + iphdrlen + sizeof(UDP_HDR));
		data_size = (Size - sizeof(ETHER_HDR) - iphdrlen - sizeof(UDP_HDR));

		fprintf(logfile, "\n\n***********************UDP Packet*************************\n");

		PrintIpHeader(Buffer, Size);

		fprintf(logfile, "\nUDP Header\n");
		fprintf(logfile, " |-Source Port : %d\n", ntohs(udpheader->source_port));
		fprintf(logfile, " |-Destination Port : %d\n", ntohs(udpheader->dest_port));
		fprintf(logfile, " |-UDP Length : %d\n", ntohs(udpheader->udp_length));
		fprintf(logfile, " |-UDP Checksum : %d\n", ntohs(udpheader->udp_checksum));

		fprintf(logfile, "\n");

		fprintf(logfile, "IP Header\n");
		PrintData((u_char*)iphdr, iphdrlen);

		fprintf(logfile, "UDP Header\n");
		PrintData((u_char*)udpheader, sizeof(UDP_HDR));

		fprintf(logfile, "Data Payload\n");
		PrintData(data, data_size);

		fprintf(logfile, "\n###########################################################\n");
	}
};

class ICMP : public Packets {
public:
	void printPacket(u_char* Buffer, int Size) {
		int iphdrlen = 0, icmphdrlen = 0, data_size = 0;

		iphdr = (IPV4_HDR*)(Buffer + sizeof(ETHER_HDR));
		iphdrlen = iphdr->ip_header_len * 4;

		icmpheader = (ICMP_HDR*)(Buffer + iphdrlen + sizeof(ETHER_HDR));

		data = (Buffer + sizeof(ETHER_HDR) + iphdrlen + sizeof(ICMP_HDR));
		data_size = (Size - sizeof(ETHER_HDR) - iphdrlen - sizeof(ICMP_HDR));

		fprintf(logfile, "\n\n***********************ICMP Packet*************************\n");
		PrintIpHeader(Buffer, Size);

		fprintf(logfile, "\n");

		fprintf(logfile, "ICMP Header\n");
		fprintf(logfile, " |-Type : %d", (unsigned int)(icmpheader->type));

		if ((unsigned int)(icmpheader->type) == 11)
		{
			fprintf(logfile, " (TTL Expired)\n");
		}
		else if ((unsigned int)(icmpheader->type) == 0)
		{
			fprintf(logfile, " (ICMP Echo Reply)\n");
		}

		fprintf(logfile, " |-Code : %d\n", (unsigned int)(icmpheader->code));
		fprintf(logfile, " |-Checksum : %d\n", ntohs(icmpheader->checksum));
		fprintf(logfile, " |-ID : %d\n", ntohs(icmpheader->id));
		fprintf(logfile, " |-Sequence : %d\n", ntohs(icmpheader->seq));
		fprintf(logfile, "\n");

		fprintf(logfile, "IP Header\n");
		PrintData((u_char*)iphdr, iphdrlen);

		fprintf(logfile, "ICMP Header\n");
		PrintData((u_char*)icmpheader, sizeof(ICMP_HDR));

		fprintf(logfile, "Data Payload\n");
		PrintData(data, data_size);

		fprintf(logfile, "\n###########################################################\n");
	}
};
