/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_dict_full.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 21:54:54 by hle-hena          #+#    #+#             */
/*   Updated: 2025/08/28 10:11:10 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void	is_value_missing(int value, char *identifier, int id, int *missing)
{
	char	letter[2];

	if (value == -1)
	{
		letter[0] = id;
		letter[1] = 0;
		*missing = 1;
		ft_perror(-1, ft_strsjoin((char *[]){"Missing ", identifier,
				"\tfor id ", letter, NULL}), 1);
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
		ft_perror(-1, ft_strsjoin((char *[]){"Missing ", identifier,
				"\tfor id ", letter, NULL}), 1);
	}
}

int	has_walls(t_tile *tile, int id)
{
	char	letter[2];
	int		missing;

	missing = 0;
	if (!tile->wpath)
	{
		letter[0] = id;
		letter[1] = 0;
		ft_perror(-1, ft_strsjoin((char *[]){"Missing walls\tfor id ",
				letter, NULL}), 1);
		return (1);
	}
	is_text_missing(tile->wall.img, "wt", id, &missing);
	is_value_missing(tile->is_wall, "W", id, &missing);
	is_text_missing(tile->tex_ce.img, "C", id, &missing);
	is_text_missing(tile->tex_fl.img, "F", id, &missing);
	return (missing);
}

int	is_replace_tile_missing(t_tile **tiles, t_map *map)
{
	if (map->replace_tile == map->void_char && !tiles['0'])
		return (ft_perror(-1, "Map is missing the player replace tile.\nYou \
should add something like 'P=0' before the start of the map.", 0), 1);
	else if (map->replace_tile == map->void_char)
		map->replace_tile = '0';
	if (tiles[(int)map->replace_tile]->is_wall)
		return (ft_perror(-1, "The player replace tile should not be a wall \
tile.", 0), 1);
	return (0);
}

int	is_dict_full(t_map *map, int err)
{
	t_tile	**tiles;
	size_t	i;

	if (err)
		return (1);
	i = -1;
	tiles = get_tile_dict();
	while (++i < 256)
	{
		if (tiles[i])
		{
			if (!tiles[i]->wpath)
			{
				add_base_wall(tiles[i], &err);
				if (err)
					return (1);
			}
			else if (has_walls(tiles[i], i))
				return (1);
		}
	}
	return (is_replace_tile_missing(tiles, map));
}
