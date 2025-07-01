/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_dict_full.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 21:54:54 by hle-hena          #+#    #+#             */
/*   Updated: 2025/07/01 10:45:55 by hle-hena         ###   ########.fr       */
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

void	fill_preset(t_tile **tiles)
{
	t_tile	*tile;

	tile = tiles['0'];
	if (tile)
	{
		if (tile->is_wall == -1)
			tile->is_wall = 0;
	}
	tile = tiles['1'];
	if (tile)
	{
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
should add something like 'P=0' before the start of the map.", 0), *missing = 1,
			VOID);
	else if (map->replace_tile == map->void_char)
		map->replace_tile = '0';
	if (tiles[(int)map->replace_tile]->is_wall)
		return (ft_perror(-1, "The player replace tile should not be a wall \
tile.", 0), *missing = 1, VOID);
}

void	add_wpath(t_tile *tile, int *err)
{
	t_wpath	*wpath;

	if (tile->wpath)
		return ;
	wpath = malloc(sizeof(t_wpath));
	if (!wpath)
		return (*err = 1, VOID);
	*wpath = (t_wpath){(t_vec){0, 0}, (t_vec){0, 1}, (t_vec){0}, tile->tex_we,
			(t_vec){-1, 0}, tile->tex_we.reflectance, 0, 0};
	add_link(&tile->wpath, wpath);
	wpath = malloc(sizeof(t_wpath));
	if (!wpath)
		return (*err = 1, VOID);
	*wpath = (t_wpath){(t_vec){0, 0}, (t_vec){1, 0}, (t_vec){0}, tile->tex_no,
			(t_vec){0, -1}, tile->tex_no.reflectance, 0, 0};
	add_link(&tile->wpath, wpath);
	wpath = malloc(sizeof(t_wpath));
	if (!wpath)
		return (*err = 1, VOID);
	*wpath = (t_wpath){(t_vec){1, 1}, (t_vec){0, 1}, (t_vec){0}, tile->tex_so,
			(t_vec){0, 1}, tile->tex_so.reflectance, 0, 0};
	add_link(&tile->wpath, wpath);
	wpath = malloc(sizeof(t_wpath));
	if (!wpath)
		return (*err = 1, VOID);
	*wpath = (t_wpath){(t_vec){1, 1}, (t_vec){1, 0}, (t_vec){0}, tile->tex_ea,
			(t_vec){1, 0}, tile->tex_ea.reflectance, 0, 0};
	add_link(&tile->wpath, wpath);
}

int	is_dict_full(t_map *map, int err)
{
	t_tile	**tiles;
	size_t	i;
	int		missing;

	if (err)
		return (1);
	i = -1;
	missing = 0;
	tiles = get_tile_dict();
	fill_preset(tiles);
	while (++i < 256)
	{
		if (tiles[i])
		{
			if (!tiles[i]->wpath)
			{
				add_wpath(tiles[i], &err);
				if (err)
					return (ft_perror(-1, "Internal error : malloc", 0), 1);
			}
			is_text_missing(tiles[i]->wpath, "the walls", i, &missing);
			is_text_missing(tiles[i]->tex_ce.img, "C", i, &missing);
			is_text_missing(tiles[i]->tex_fl.img, "F", i, &missing);
			is_value_missing(tiles[i]->is_wall, "W", i, &missing);
		}
	}
	is_replace_tile_missing(tiles, map, &missing);
	return (missing);
}
