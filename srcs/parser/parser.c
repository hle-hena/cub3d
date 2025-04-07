/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 16:16:19 by hle-hena          #+#    #+#             */
/*   Updated: 2025/04/04 16:43:33 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

t_map	*get_map(void)
{
	static t_map	map = (t_map){0};

	return (&map);
}

t_map	load_map(int ac, char **av)
{
	t_map	*map;
	char	*map_name;
	char	*exetension;

	if (ac != 2)
		ft_perror(1, ft_strdup("Invalid usage. Correct :\n\t./cub3d map_name.be\
r"), 0);
	map_name = av[1];
	exetension = ft_strrchr(map_name, '.');
	map = get_map();
	retrieve_map_info(map, map_name);
	// pars_file(map, map_name);
}