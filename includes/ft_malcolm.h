/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malcolm.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hulefevr <hulefevr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 12:03:00 by hulefevr          #+#    #+#             */
/*   Updated: 2025/08/07 12:13:36 by hulefevr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_MALCOLM_H
#define FT_MALCOLM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <ctype.h>
#include <stddef.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netpacket/packet.h>
#include <net/ethernet.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <netinet/if_ether.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <fcntl.h>


#include <ifaddrs.h>

#define MAC_LEN 6

// LIBFT

int			ft_strtol(const char *str, int base);
int			ft_isxdigit(int c);
void		*ft_memcpy(void *dest, const void *src, size_t n);
size_t		ft_strlcpy(char *dst, const char *src, size_t dstsize);
void		*ft_memset(void *s, int c, size_t n);
int			ft_memcmp(const void *s1, const void *s2, size_t n);
ssize_t		ft_strlen(const char *s);
uint32_t	ft_ntohl(uint32_t netlong);

// Global flag to catch SIGINT
extern volatile sig_atomic_t running;

// Signal handling
void setup_signal_handler(void);

// Parsing
int parse_mac(const char *mac_str, unsigned char *mac_bytes);

// Interface
int find_matching_interface(const struct in_addr *target_ip, char *if_name);

// Socket
int open_arp_socket(void);

// ARP
int wait_for_arp_request(int sockfd, const struct in_addr *spoofed_ip,
						 unsigned char *target_mac_out, unsigned char *target_ip_out);
int send_arp_reply(int sockfd, const char *if_name,
				   const unsigned char *src_mac,
				   const struct in_addr *src_ip,
				   const unsigned char *target_mac,
				   const struct in_addr *target_ip);

int send_arp_reply(int sockfd, const char *if_name,
                   const unsigned char *src_mac,
                   const struct in_addr *src_ip,
                   const unsigned char *target_mac,
                   const struct in_addr *target_ip);

#endif
