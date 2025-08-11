/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hulefevr <hulefevr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 12:00:11 by hulefevr          #+#    #+#             */
/*   Updated: 2025/08/07 12:10:00 by hulefevr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_malcolm.h"

int	ft_isxdigit(int c)
{
	if ((c >= '0' && c <= '9') ||
		(c >= 'a' && c <= 'f') ||
		(c >= 'A' && c <= 'F'))
		return (1);
	return (0);
}


void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	int		i;
	char	*s1;
	char	*s2;

	s1 = (char *)dest;
	s2 = (char *)src;
	i = 0;
	if (!s1 && !s2)
		return (NULL);
	while (n > 0)
	{
		s1[i] = s2[i];
		i++;
		n--;
	}
	return (dest);
}

size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t	i;

	i = 0;
	if (dstsize > 0)
	{
		while (src[i] && --dstsize)
		{
			dst[i] = src[i];
			i++;
		}
		dst[i] = '\0';
	}
	while (src[i])
		i++;
	return (i);
}

int	ft_strtol(const char *str, int base)
{
	int	result = 0;
	int	value;
	char	c;

	if (base != 16) // pour l’instant on ne supporte que l’hex
		return (-1);

	while ((c = *str++))
	{
		if (c >= '0' && c <= '9')
			value = c - '0';
		else if (c >= 'a' && c <= 'f')
			value = 10 + (c - 'a');
		else if (c >= 'A' && c <= 'F')
			value = 10 + (c - 'A');
		else
			return (-1); // caractère non valide

		result = result * 16 + value;
	}

	return result;
}

void	*ft_memset(void *s, int c, size_t n)
{
	unsigned char *p = s;

	while (n--)
		*p++ = (unsigned char)c;
	return s;
}

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	const unsigned char *a = s1;
	const unsigned char *b = s2;

	while (n--) {
		if (*a != *b)
			return (*a - *b);
		a++;
		b++;
	}
	return 0;
}

ssize_t	ft_strlen(const char *s)
{
	size_t len = 0;

	while (s[len] != '\0')
		len++;
	return len;
}