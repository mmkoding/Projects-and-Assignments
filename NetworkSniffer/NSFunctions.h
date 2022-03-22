#pragma once
#include "NSVarsAndStructs.h"
#include "GatherPackets.h"

void print_ethernet_header(u_char* buffer)
{
	ETHER_HDR* eth = (ETHER_HDR*)buffer;

	fprintf(logfile, "\n");
	fprintf(logfile, "Ethernet Header\n");
	fprintf(logfile, " |-Destination Address : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X \n", eth->dest[0], eth->dest[1], eth->dest[2], eth->dest[3], eth->dest[4], eth->dest[5]);
	fprintf(logfile, " |-Source Address      : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X \n", eth->source[0], eth->source[1], eth->source[2], eth->source[3], eth->source[4], eth->source[5]);
	fprintf(logfile, " |-Protocol            : 0x%.4x \n", ntohs(eth->type));
}

void assignProtocol(Packets* data, unsigned char ipProtID, u_char* Buffer, int Size) {
	switch (ipProtID) //Check the Protocol and do accordingly...
	{
	case 1: //ICMP Protocol
		icmp++;
		data = new ICMP();
		data->printPacket(Buffer, Size);
		break;

	case 2: //IGMP Protocol
		igmp++;
		break;

	case 6: //TCP Protocol
		tcp++;
		data = new TCP();
		data->printPacket(Buffer, Size);
		break;

	case 17: //UDP Protocol
		udp++;
		data = new UDP();
		data->printPacket(Buffer, Size);
		break;

	default: //Some Other Protocol like ARP etc.
		others++;
		break;
	}


}

void ProcessPacket(u_char* Buffer, int Size, u_int packnum)
{
	//Ethernet header
	ethhdr = (ETHER_HDR*)Buffer;


	//Ip packets
	if (ntohs(ethhdr->type) == 0x0800)
	{
		//ip header
		iphdr = (IPV4_HDR*)(Buffer + sizeof(ETHER_HDR));

		Packets* printMe = new Packets();
		unsigned char ipprot;

		/*
		if (packnum == 1) {
			ipprot = iphdr->ip_protocol;
		}
		else if (packnum == 2 && iphdr->ip_protocol == 17) {//udp
			ipprot = 17;
		}
		else if (packnum == 3 && iphdr->ip_protocol == 6) {//tcp
			ipprot = 6;
		}
		else if (packnum == 4 && iphdr->ip_protocol == 1) {//icmp
			ipprot = 1;
		}
		else {
			return;
		}
		*/
		
		total++;

		assignProtocol(printMe, iphdr->ip_protocol, Buffer, Size);
		//printMe->printPacket(Buffer, Size);
		
	}
	
	//printf("TCP : %d UDP : %d ICMP : %d IGMP : %d Others : %d Total : %d\r", tcp, udp, icmp, igmp, others, total);
	std::cout << "TCP: " << tcp << " UDP " << udp << " ICMP: " << icmp << " IGMP: " << igmp << " Others: " << others << " Total: " << total << "\r";
}



