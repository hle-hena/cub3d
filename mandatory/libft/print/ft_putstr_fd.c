/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 14:02:40 by hle-hena          #+#    #+#             */
/*   Updated: 2025/01/13 13:33:25 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

int	ft_putstr_fd(char *str, int fd)
{
	int	len;

	len = 0;
	if (!str)
		return (len);
	while (*str)
	{
		if (write (fd, str++, 1) == -1)
			return (len);
		len++;
	}
	return (len);
}
