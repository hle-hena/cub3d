/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_perror.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 12:07:23 by hle-hena          #+#    #+#             */
/*   Updated: 2025/09/11 15:24:28 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	put_error_code(int code)
{
	if (code == 1)
		ft_putstr_fd("Something went wrong.", 2);
	else if (code == 2)
		ft_putstr_fd("Wrong argument given to the program.", 2);
	else if (code == 3)
		ft_putstr_fd("Error happened during a malloc.", 2);
	else if (code == 4)
		ft_putstr_fd("Something went wrong during the parsing of the args.", 2);
}

void	ft_perror(int error_code, char *custom_mess, int clean)
{
	if (custom_mess)
	{
		ft_putstr_fd("\001\033[38;2;255;0;0m\002", 2);
		ft_putstr_fd("Error\n", 2);
		ft_putstr_fd(custom_mess, 2);
		ft_putstr_fd("\001\033[0;0m\002\n", 2);
	}
	if (clean)
		ft_del((void **)&custom_mess);
	if (error_code >= 0)
		exit(error_code);
}
