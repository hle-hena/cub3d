/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_map_valid.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 18:45:08 by hle-hena          #+#    #+#             */
/*   Updated: 2025/04/23 16:28:54 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

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
		if (get(map, x - 1, y) != ' ')
			open += !tiles[get(map, x - 1, y)]->is_wall;
	}
	if (x < map->wid - 2)
	{
		if (get(map, x + 1, y) != ' ')
			open += !tiles[get(map, x + 1, y)]->is_wall;
	}
	if (y >= 1)
	{
		if (get(map, x, y - 1) != ' ')
			open += !tiles[get(map, x, y - 1)]->is_wall;
	}
	if (y < map->len - 2)
	{
		if (get(map, x, y + 1) != ' ')
			open += !tiles[get(map, x, y + 1)]->is_wall;
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
			if (get(mp, x, y) == mp->void_char)
			{
				if (check_neighbors(mp, tiles, x, y))
					return (print_map_open(x + 1, y + 1), 1);
			} 
			else if ((x == 0 || x == mp->wid - 1 || y == 0 || y == mp->len - 1)
				&& tiles[get(mp, x, y)]->is_wall != 1)
				return (print_map_open(x + 1, y + 1), 1);
		}
	}
	return (0);
}
