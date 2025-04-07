/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 16:05:37 by hle-hena          #+#    #+#             */
/*   Updated: 2025/04/04 16:33:15 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

t_data	**get_data(void)
{
	static t_data	data = (t_data){0};

	return (&data);
}

void	clean_data(void)
{
	return ;
}

int	main(int ac, char **av)
{
	t_data	*data;

	data->map = load_map(ac, av);
	clean_data();
	return (0);
}