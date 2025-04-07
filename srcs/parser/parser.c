/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 16:16:19 by hle-hena          #+#    #+#             */
/*   Updated: 2025/04/07 16:35:01 by hle-hena         ###   ########.fr       */
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
	char	*extension;
	int		err;

	if (ac != 2)
		ft_perror(1, ft_strdup("Invalid usage. Correct :\n\t./cub3d map_name.cu\
b"), 0);
	map_name = av[1];
	extension = ft_strrchr(map_name, '.');
	if (!extension)
		ft_perror(1, ft_strdup("Invalid usage. File name should be .cub"), 0);
	if (ft_strncmp(".cub", extension, 5))
		ft_perror(1, ft_strdup("Invalid usage. File name should be .cub"), 0);
	map = get_map();
	map->floor.bl = -1;
	map->ceiling.bl = -1;
	retrieve_map_info(&map, map_name, &err);
	if (err)
		return ()
	// pars_file(map, map_name);
}