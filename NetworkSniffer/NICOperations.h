#pragma once
#include "NSVarsAndStructs.h"
#include "NSFunctions.h"




class NICOperations {
	private:
		

	public:
		

		int getNIC() {
			u_int i, res, inum, packnum;
			u_char errbuf[PCAP_ERRBUF_SIZE], buffer[100];
			u_char* pkt_data;
			time_t seconds;
			struct tm tbreak;
			pcap_if_t* alldevs, * d;
			pcap_t* fp;
			struct pcap_pkthdr* header;


			fopen_s(&logfile, "log.txt", "w");

			if (logfile == NULL)
			{
				printf("Unable to create file.");
			}

			/* The user didn't provide a packet source: Retrieve the local device list */
			if (pcap_findalldevs_ex((char*)PCAP_SRC_IF_STRING, NULL, &alldevs, (char*)errbuf) == -1)
			{
				fprintf(stderr, "Error in pcap_findalldevs_ex: %s\n", errbuf);
				return -1;
			}

			i = 0;
			/* Print the list */
			for (d = alldevs; d; d = d->next)
			{
				printf("%d. %s\n    ", ++i, d->name);

				if (d->description)
				{
					std::cout << d->description << std::endl;
				}
				else
				{
					std::cout << " (No description available) " << std::endl;
				}
			}

			if (i == 0)
			{
				fprintf(stderr, "No interfaces found! Exiting.\n");
				return -1;
			}

			printf("Enter the interface number you would like to sniff : ");
			std::cin >> inum;


			/* Jump to the selected adapter */
			for (d = alldevs, i = 0; i < inum - 1; d = d->next, i++);

			/* Open the device */
			if ((fp = pcap_open(d->name,
				100 /*snaplen*/,
				PCAP_OPENFLAG_PROMISCUOUS /*flags*/,
				20 /*read timeout*/,
				NULL /* remote authentication */,
				(char*)errbuf)
				) == NULL)
			{
				fprintf(stderr, "\nError opening adapter\n");
				return -1;
			}

			/*
			std::cout << "Which one of these packets would you like to sniff?" << std::endl;
			std::cout << "1. All of them" << std::endl;
			std::cout << "2. UDP" << std::endl;
			std::cout << "3. TCP" << std::endl;
			std::cout << "4. ICMP" << std::endl;




			std::cin >> packnum;
			if (packnum < 1 || packnum > 4) {
				std::cout << std::endl;
				std::cout << "PLEASE ENTER A VALID NUMBER!!" << std::endl;
				return -1;
			}
			*/

			//read packets in a loop 
			while ((res = pcap_next_ex(fp, &header, (const u_char**)&pkt_data)) >= 0)
			{
				if (res == 0)
				{
					// Timeout elapsed
					continue;
				}
				seconds = header->ts.tv_sec;
				localtime_s(&tbreak, &seconds);
				strftime((char*)buffer, 80, "%d-%b-%Y %I:%M:%S %p", &tbreak);
				//print pkt timestamp and pkt len
				//fprintf(logfile , "\nNext Packet : %ld:%ld (Packet Length : %ld bytes) " , header->ts.tv_sec, header->ts.tv_usec, header->len);
				fprintf(logfile, "\nNext Packet : %s.%ld (Packet Length : %ld bytes) ", buffer, header->ts.tv_usec, header->len);
				ProcessPacket(pkt_data, header->caplen, 1);
			}

			if (res == -1)
			{
				fprintf(stderr, "Error reading the packets: %s\n", pcap_geterr(fp));
				return -1;
			}

			return 0;

		}

		
	
};
