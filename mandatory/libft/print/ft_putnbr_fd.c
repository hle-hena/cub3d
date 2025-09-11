/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 14:01:43 by hle-hena          #+#    #+#             */
/*   Updated: 2025/01/13 13:44:37 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

int	ft_numlen(long int num)
{
	int	len;

	len = 0;
	num = (num > 0) * num + (num < 0) * (-num);
	while (num > 9)
	{
		len ++;
		num /= 10;
	}
	len ++;
	return (len);
}

int	ft_putnbr_fd(int n, int fd)
{
	long long int	nb;

	nb = n;
	if (nb < 0)
	{
		nb = -nb;
		ft_putchar_fd('-', fd);
	}
	if (nb > 9)
		ft_putnbr_fd(nb / 10, fd);
	ft_putchar_fd(nb % 10 + '0', fd);
	return (ft_numlen(nb));
}

int	ft_numlen_base(unsigned long nb, char *base)
{
	int				len;
	unsigned int	baselen;

	len = 0;
	baselen = ft_strlen(base);
	while (nb > baselen - 1)
	{
		len ++;
		nb /= baselen;
	}
	len ++;
	return (len);
}

int	ft_putnbr_base_fd(unsigned long nb, char *base, int fd)
{
	int				len;
	unsigned long	baselen;

	len = 0;
	baselen = ft_strlen(base);
	if (nb > baselen - 1)
		len += ft_putnbr_base_fd(nb / baselen, base, fd);
	len += ft_putchar_fd(base[nb % baselen], fd);
	return (len);
}
