/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hulefevr <hulefevr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 11:26:47 by hulefevr          #+#    #+#             */
/*   Updated: 2025/08/07 12:09:53 by hulefevr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_malcolm.h"

int	parse_mac(const char *mac_str, unsigned char *mac_bytes)
{
	if (ft_strlen(mac_str) != 17) {
		return -1; // Invalid MAC address length
	}
	for (int i = 0; i < MAC_LEN; i++) {
		char byte_str[3] = {mac_str[i*3], mac_str[i*3 + 1], '\0'};
		if (!ft_isxdigit(byte_str[0]) || !ft_isxdigit(byte_str[1])) {
			return -1; // Invalid character in MAC address
		}
		mac_bytes[i] = (unsigned char)ft_strtol(byte_str, 16);
		if (i < 5 && mac_str[i*3 + 2] != ':') {
			return -1; // Missing colon separator
		}
	}
	return 0; // Success
}