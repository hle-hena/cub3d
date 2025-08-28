/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_map_valid.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 18:45:08 by hle-hena          #+#    #+#             */
/*   Updated: 2025/08/28 11:06:40 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

int	light_check(t_map *map)
{
	t_data	*data;
	t_lmap	lmap;
	int		i;

	data = get_data();
	lmap = data->lmap;
	if (lmap.lights)
	{
		i = -1;
		while (++i < lmap.nb_ls)
		{
			if (lmap.lights[i].pos.x >= map->wid
				|| lmap.lights[i].pos.y >= map->len)
				return (ft_perror(-1, "A light is outsided the map.", 0), 1);
		}
	}
	return (0);
}

void	print_map_open(int x, int y)
{
	char	*x_str;
	char	*y_str;

	x_str = ft_itoa(x);
	y_str = ft_itoa(y);
	ft_perror(-1, ft_strsjoin((char *[]){"Map is open at (", x_str, ", ", y_str,
			").", NULL}), 1);
	ft_del((void **)&x_str);
	ft_del((void **)&y_str);
}

int	check_neighbors(t_map *map, t_tile **tiles, int x, int y)
{
	int	open;

	open = 0;
	if (x >= 1)
	{
		if (*(map->map + y * map->wid + x - 1) != ' ')
			open += !tiles[*(map->map + y * map->wid + x - 1)]->is_wall;
	}
	if (x < map->wid - 2)
	{
		if (*(map->map + y * map->wid + x + 1) != ' ')
			open += !tiles[*(map->map + y * map->wid + x + 1)]->is_wall;
	}
	if (y >= 1)
	{
		if (*(map->map + (y - 1) * map->wid + x) != ' ')
			open += !tiles[*(map->map + (y - 1) * map->wid + x)]->is_wall;
	}
	if (y < map->len - 2)
	{
		if (*(map->map + (y + 1) * map->wid + x) != ' ')
			open += !tiles[*(map->map + (y + 1) * map->wid + x)]->is_wall;
	}
	return (open);
}

int	is_map_valid(t_map *mp, t_tile **tiles, int err)
{
	int	x;
	int	y;

	if (err)
		return (1);
	y = -1;
	if (mp->player.y == -1)
		return (ft_perror(-1, "Player is missing.", 0), 1);
	while (++y < mp->len)
	{
		x = -1;
		while (++x < mp->wid)
		{
			if (*(mp->map + y * mp->wid + x) == mp->void_char)
			{
				if (check_neighbors(mp, tiles, x, y))
					return (print_map_open(x + 1, y + 1), 1);
			}
			else if ((x == 0 || x == mp->wid - 1 || y == 0 || y == mp->len - 1)
				&& tiles[*(mp->map + y * mp->wid + x)]->is_wall != 1)
				return (print_map_open(x + 1, y + 1), 1);
		}
	}
	return (light_check(mp));
}
