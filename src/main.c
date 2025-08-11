/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hulefevr <hulefevr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 11:21:23 by hulefevr          #+#    #+#             */
/*   Updated: 2025/08/07 11:26:35 by hulefevr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_malcolm.h"

volatile sig_atomic_t running = 1;

int	main(int ac, char **av)
{
	if (ac != 5) {
		fprintf(stderr, "Usage: %s <source_ip> <source_max> <target_ip> <target_mac>\n", av[0]);
		return EXIT_FAILURE;
	}

	struct in_addr source_ip, target_ip;
	unsigned char target_mac[MAC_LEN], src_mac[MAC_LEN];
	char if_name[IFNAMSIZ] = {0};

	if (inet_pton(AF_INET, av[1], &source_ip) != 1 ||
		parse_mac(av[2], src_mac) != 0 ||
		inet_pton(AF_INET, av[3], &target_ip) != 1 ||
	    parse_mac(av[4], target_mac) != 0)
	{
		fprintf(stderr, "ft_malcolm: invalid IP or MAC address.\n");
        return EXIT_FAILURE;
	}

	setup_signal_handler();

	if (find_matching_interface(&target_ip, if_name) != 0) {
		fprintf(stderr, "ft_malcolm: no matching interface found for target IP.\n");
		return EXIT_FAILURE;
	}

	printf("Using interface: %s\n", if_name);

	int sockfd = open_arp_socket();
	if (sockfd < 0) {
		fprintf(stderr, "ft_malcolm: failed to open ARP socket.\n");
		return EXIT_FAILURE;
	}
	
	if (wait_for_arp_request(sockfd, &source_ip, target_mac, (unsigned char*)&target_ip) == 0) {
        send_arp_reply(sockfd, if_name, src_mac, &source_ip, target_mac, &target_ip);
    }

	close(sockfd);
	printf("ft_malcolm: finished successfully.\n");
	return EXIT_SUCCESS;
}
