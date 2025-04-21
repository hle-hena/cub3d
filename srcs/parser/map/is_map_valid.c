/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_map_valid.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 18:45:08 by hle-hena          #+#    #+#             */
/*   Updated: 2025/04/21 19:58:55 by hle-hena         ###   ########.fr       */
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

int	check_neighbors(t_map *map, int x, int y)
{
	int	open;

	open = 0;
	if (x >= 1)
	{
		if (map->matrix[y][x - 1] != ' ')
			open += !map->tiles[(int)map->matrix[y][x - 1]]->is_wall;
	}
	if (x < map->wid - 2)
	{
		if (map->matrix[y][x + 1] != ' ')
			open += !map->tiles[(int)map->matrix[y][x + 1]]->is_wall;
	}
	if (y >= 1)
	{
		if (map->matrix[y - 1][x] != ' ')
			open += !map->tiles[(int)map->matrix[y - 1][x]]->is_wall;
	}
	if (y < map->len - 2)
	{
		if (map->matrix[y + 1][x] != ' ')
			open += !map->tiles[(int)map->matrix[y + 1][x]]->is_wall;
	}
	return (open);
}

int	is_map_valid(t_map *map, int err)
{
	int	x;
	int	y;

	if (err)
		return (1);
	y = -1;
	if (map->player.y == -1)
		return (ft_perror(-1, "Player is missing.", 0), 1);
	while (++y < map->len)
	{
		x = -1;
		while (++x < map->wid)
		{
			if (map->matrix[y][x] == map->void_char)
			{
				if (check_neighbors(map, x, y))
					return (print_map_open(x + 1, y + 1), 1);
			} 
			else if (map->tiles[(int)map->matrix[y][x]]->is_wall != 1 && (x == 0
					|| x == map->wid - 1 || y == 0 || y == map->len - 1))
				return (print_map_open(x + 1, y + 1), 1);
		}
	}
	return (0);
}
