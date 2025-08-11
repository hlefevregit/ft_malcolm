/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hulefevr <hulefevr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 11:30:03 by hulefevr          #+#    #+#             */
/*   Updated: 2025/08/07 12:11:10 by hulefevr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_malcolm.h"

int find_matching_interface(const struct in_addr *target_ip, char *if_name) {
	struct ifaddrs *ifaddr = NULL, *ifa = NULL;

	if (getifaddrs(&ifaddr) == -1)
        return -1;

	uint32_t t = ft_ntohl(target_ip->s_addr);


	for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
		if (!ifa->ifa_addr || ifa->ifa_addr->sa_family != AF_INET)
			continue;
		
		struct sockaddr_in *addr = (struct sockaddr_in *)ifa->ifa_addr;
		struct sockaddr_in *netmask = (struct sockaddr_in *)ifa->ifa_netmask;
		
		if (!netmask) continue;

		uint32_t ip = ft_ntohl(addr->sin_addr.s_addr);
		uint32_t mask = ft_ntohl(netmask->sin_addr.s_addr);

		if ((ip & mask) == (t & mask)) {
			ft_strlcpy(if_name, ifa->ifa_name, IFNAMSIZ - 1);
			// if_name[IFNAMSIZ - 1] = '\0'; // Ensure null termination
			freeifaddrs(ifaddr);
			return 0; // Found matching interface
		}
	}

	freeifaddrs(ifaddr);
	printf("[DEBUG] No matching interface found for IP %s\n", inet_ntoa(*target_ip));
	return -1; // No matching interface found
}

int open_arp_socket(void) {
	int sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ARP));
	if (sockfd < 0) {
		fprintf(stderr, "ft_malcolm: socket creation failed: %s\n", strerror(errno));
		return -1;
	}
	return sockfd;
}
void	handle_sigint(int sig)
{
	(void)sig;
	running = 0;
	printf("\nReceived Ctrl+C. Exiting...\n");
}

void	setup_signal_handler(void)
{
	struct sigaction sa;

	ft_memset(&sa, 0, sizeof(sa)); 
	sa.sa_handler = handle_sigint;

	if (sigaction(SIGINT, &sa, NULL) == -1) {
		printf("ft_malcolm: failed to set signal handler.\n");
	}
}