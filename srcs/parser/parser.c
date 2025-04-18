/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 16:16:19 by hle-hena          #+#    #+#             */
/*   Updated: 2025/04/13 11:05:36 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

t_map	*get_map(void)
{
	static t_map	map = (t_map){0};

	return (&map);
}

void	check_map_validity(t_map *map, int *err)
{
	int	x;
	int	y;

	if (map->player.y == -1)
		return (*err = 1, ft_perror(-1, "Missing player.", 0), (void)0);
	y = -1;
	while (++y < map->len)
	{
		if (map->matrix[y][0] != '1')
			return (ft_perror(-1, "Map is not closed on the left edge.", 0),
				*err = 1, (void)0);
		if (map->matrix[y][map->wid - 1] != '1')
			return (ft_perror(-1, "Map is not closed on the right edge.", 0),
				*err = 1, (void)0);
	}
	x = -1;
	while (++x < map->wid)
	{
		if (map->matrix[0][x] != '1')
			return (ft_perror(-1, "Map is not closed on the top edge.", 0),
				*err = 1, (void)0);
		if (map->matrix[map->len - 1][x] != '1')
			return (ft_perror(-1, "Map is not closed on the bottom edge.", 0),
				*err = 1, (void)0);
	}
}

int	get_map_fd(char *map_name)
{
	char	*extension;
	int		map_fd;

	extension = ft_strrchr(map_name, '.');
	if (!extension)
		ft_perror(1, "Invalid usage. File name should be .cub", clean_data());
	if (ft_strncmp(".cub", extension, 5))
		ft_perror(1, "Invalid usage. File name should be .cub", clean_data());
	map_fd = open(map_name, O_RDONLY, 0777);
	if (map_fd == -1)
		ft_perror(1, "Map not found.", clean_data());
	return (map_fd);
}

t_map	*load_map(int ac, char **av)
{
	t_map	*map;
	int		map_fd;
	int		err;

	err = 0;
	if (ac != 2)
		ft_perror(1, "Invalid usage. Correct :\n\t./cub3d map_name.cub",
			clean_data());
	map_fd = get_map_fd(av[1]);
	map = get_map();
	map->floor.bl = -1;
	map->ceiling.bl = -1;
	map->player.y = -1;
	retrieve_map_info(map, map_fd, &err);
	if (err)
		ft_perror(1, NULL, clean_data());
	retrieve_map_grid(map, map_fd, &err);
	if (err)
		ft_perror(1, NULL, clean_data());
	check_map_validity(map, &err);
	if (err)
		ft_perror(1, NULL, clean_data());
	map->player.x += 0.5;
	map->player.y += 0.5;
	return (close(map_fd), map);
}
