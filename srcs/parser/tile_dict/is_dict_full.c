/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_dict_full.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 21:54:54 by hle-hena          #+#    #+#             */
/*   Updated: 2025/04/22 11:56:06 by hle-hena         ###   ########.fr       */
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
		ft_perror(-1, ft_strsjoin((char *[]){"Missing ", identifier, "\tfor id ",
			letter, NULL}), 1);
	}
}

void	is_text_missing(void *img, char *identifier, int id, int *missing)
{
	char	letter[2];

	if (!img)
	{
		letter[0] = id;
		letter[1] = 0;
		*missing = 1;
		ft_perror(-1, ft_strsjoin((char *[]){"Missing ", identifier, "\tfor id ",
			letter, NULL}), 1);
	}
}

void	fill_preset(t_map *map)
{
	t_tile	*tile;

	tile = map->tiles['0'];
	if (tile)
	{
		if (tile->ceil_height == -1)
			tile->ceil_height = 100;
		if (tile->floor_height == -1)
			tile->floor_height = 0;
		if (tile->is_wall == -1)
			tile->is_wall = 0;
	}
	tile = map->tiles['1'];
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

void	is_replace_tile_missing(t_map *map, int *missing)
{
	if (*missing)
		return ;
	if (map->replace_tile == map->void_char && !map->tiles['0'])
		return (ft_perror(-1, "Map is missing the player replace tile.\nYou \
should add something like 'P=0' before starting the map.", 0), *missing = 1,
			VOID);
	else if (map->replace_tile == map->void_char)
		map->replace_tile = '0';
	if (map->tiles[(int)map->replace_tile]->is_wall)
		return (ft_perror(-1, "The player replace tile should not be a wall \
tile.", 0), *missing = 1, VOID);
}

int	is_dict_full(t_map *map, int err)
{
	size_t	i;
	int		missing;

	if (err)
		return (1);
	i = -1;
	missing = 0;
	fill_preset(map);
	while (++i < 256)
	{
		if (map->tiles[i])
		{
			is_text_missing(map->tiles[i]->tex_no.img, "NO", i, &missing);
			is_text_missing(map->tiles[i]->tex_so.img, "SO", i, &missing);
			is_text_missing(map->tiles[i]->tex_we.img, "WE", i, &missing);
			is_text_missing(map->tiles[i]->tex_ea.img, "EA", i, &missing);
			is_text_missing(map->tiles[i]->tex_ce.img, "C", i, &missing);
			is_text_missing(map->tiles[i]->tex_fl.img, "F", i, &missing);
			is_value_missing(map->tiles[i]->ceil_height, "CH", i, &missing);
			is_value_missing(map->tiles[i]->floor_height, "FH", i, &missing);
			is_value_missing(map->tiles[i]->is_wall, "W", i, &missing);
		}
	}
	is_replace_tile_missing(map, &missing);
	return (missing);
}
