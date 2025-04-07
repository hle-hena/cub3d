/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@students.42perpignan    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 14:38:26 by hle-hena          #+#    #+#             */
/*   Updated: 2024/11/18 18:04:01 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

static int	get_strlen(long long int n)
{
	int	len;

	len = 1;
	if (n < 0)
		n = -n;
	while (n > 9)
	{
		n /= 10;
		len ++;
	}
	return (len);
}

char	*ft_itoa(int n)
{
	char			*dest;
	long long int	nb;
	int				len;

	nb = n;
	len = get_strlen(nb) + (nb < 0);
	dest = ft_calloc(len + 1, sizeof(char));
	if (!dest)
		return (0);
	if (nb < 0)
	{
		dest[0] = '-';
		nb = -nb;
	}
	while (len -- > (dest[0] == '-'))
	{
		dest[len] = (nb % 10) + '0';
		nb /= 10;
	}
	return (dest);
}
