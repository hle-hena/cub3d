/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putendl_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 14:00:58 by hle-hena          #+#    #+#             */
/*   Updated: 2025/01/08 20:20:02 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	ft_putendl_fd(char *str, int fd)
{
	if (!str)
		return ;
	while (*str)
		if (write(fd, str++, 1) == -1)
			return ;
	if (write(fd, "\n", 1) == -1)
		return ;
}
