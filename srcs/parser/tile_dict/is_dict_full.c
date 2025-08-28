/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_dict_full.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 21:54:54 by hle-hena          #+#    #+#             */
/*   Updated: 2025/08/28 17:25:52 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	is_value_missing(int value, char *identifier, int id, int *missing)
{
	char	letter[2];

	if (value == -1)
	{
		letter[0] = id;
		letter[1] = 0;
		*missing = 1;
		ft_perror(-1, ft_strsjoin((char *[]){"Missing ", identifier, " for id ",
				letter, NULL}), 1);
	}
}

void	is_text_missing(t_img *img, char *identifier, int id, int *missing)
{
	char	letter[2];

	if (!img)
	{
		letter[0] = id;
		letter[1] = 0;
		*missing = 1;
		ft_perror(-1, ft_strsjoin((char *[]){"Missing ", identifier, " for id ",
				letter, NULL}), 1);
	}
}

void	fill_preset(t_tile **tiles)
{
	t_tile	*tile;

	tile = tiles['0'];
	if (tile)
	{
		if (tile->ceil_height == -1)
			tile->ceil_height = 100;
		if (tile->floor_height == -1)
			tile->floor_height = 0;
		if (tile->is_wall == -1)
			tile->is_wall = 0;
	}
	tile = tiles['1'];
	if (tile)
	{
		if (tile->ceil_height == -1)
			tile->ceil_height = 100;
		if (tile->floor_height == -1)
			tile->floor_height = 0;
		if (tile->is_wall == -1)
			tile->is_wall = 1;
	}
}

void	is_replace_tile_missing(t_tile **tiles, t_map *map, int *missing)
{
	if (*missing)
		return ;
	if (map->replace_tile == map->void_char && !tiles['0'])
		return (ft_perror(-1, "Map is missing the player replace tile.\nYou \
should add something like 'P=0' before starting the map.", 0), *missing = 1,
			VOID);
	else if (map->replace_tile == map->void_char)
		map->replace_tile = '0';
	if (tiles[(int)map->replace_tile]->is_wall)
		return (ft_perror(-1, "The player replace tile should not be a wall \
tile.", 0), *missing = 1, VOID);
}

int	is_dict_full(t_map *map, int i)
{
	t_tile	**tiles;
	int		missing;

	if (i)
		return (1);
	i = -1;
	missing = 0;
	tiles = get_tile_dict();
	fill_preset(tiles);
	while (++i < 256)
	{
		if (!tiles[i])
			continue ;
		is_text_missing(tiles[i]->tex_no, "NO", i, &missing);
		is_text_missing(tiles[i]->tex_so, "SO", i, &missing);
		is_text_missing(tiles[i]->tex_we, "WE", i, &missing);
		is_text_missing(tiles[i]->tex_ea, "EA", i, &missing);
		is_text_missing(tiles[i]->tex_ce, "C", i, &missing);
		is_text_missing(tiles[i]->tex_fl, "F", i, &missing);
		is_value_missing(tiles[i]->ceil_height, "CH", i, &missing);
		is_value_missing(tiles[i]->floor_height, "FH", i, &missing);
		is_value_missing(tiles[i]->is_wall, "W", i, &missing);
	}
	is_replace_tile_missing(tiles, map, &missing);
	return (missing);
}
