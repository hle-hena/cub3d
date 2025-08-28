/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   retrieve_lonely.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 10:37:56 by hle-hena          #+#    #+#             */
/*   Updated: 2025/08/28 10:21:32 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

int	lonely_type_switch(t_tile **tiles, char *line, int *err, int index)
{
	char	*arg;

	tiles[index]->is_wall = (index == '1');
	if (!line[0])
		return (0);
	arg = ft_strtrim(line + 2, " \t\n");
	if (!arg)
		return (*err = 1, 0);
	if (!*arg)
		return (ft_perror(-1, ft_strsjoin((char *[]){"Empty line after \
identifier at ", line, ".", NULL}), 1), *err = 1, 0);
	if (ft_strncmp("NO ", line, 3) == 0)
		retrieve_texture(&(tiles['1'])->tex_no, arg, err, "NO");
	else if (ft_strncmp("SO ", line, 3) == 0)
		retrieve_texture(&(tiles['1'])->tex_so, arg, err, "SO");
	else if (ft_strncmp("EA ", line, 3) == 0)
		retrieve_texture(&(tiles['1'])->tex_ea, arg, err, "EA");
	else if (ft_strncmp("WE ", line, 3) == 0)
		retrieve_texture(&(tiles['1'])->tex_we, arg, err, "WE");
	else if (ft_strncmp("C ", line, 2) == 0)
	{
		retrieve_texture(&(tiles['1'])->tex_ce, arg, err, "C");
		retrieve_texture(&(tiles['0'])->tex_ce, arg, err, "C");
	}
	else if (ft_strncmp("F ", line, 2) == 0)
	{
		retrieve_texture(&(tiles['1'])->tex_fl, arg, err, "F");
		retrieve_texture(&(tiles['0'])->tex_fl, arg, err, "F");
	}
	return (ft_del((void **)&arg), 0);
}

int	is_end(char *line)
{
	int	is_end;

	if (ft_strncmp("NO ", line, 3) == 0)
		is_end = '1';
	else if (ft_strncmp("SO ", line, 3) == 0)
		is_end = '1';
	else if (ft_strncmp("EA ", line, 3) == 0)
		is_end = '1';
	else if (ft_strncmp("WE ", line, 3) == 0)
		is_end = '1';
	else if (ft_strncmp("C ", line, 2) == 0)
		is_end = '0';
	else if (ft_strncmp("F ", line, 2) == 0)
		is_end = '0';
	else
		is_end = 1;
	return (is_end);
}

int	retrieve_lonely(t_map *map, char *line, int *err)
{
	t_tile	**tiles;
	int		temp;

	tiles = get_tile_dict();
	if (*line == '#')
		return (0);
	if (ft_strncmp(line, "P=", 2) == 0)
		return (retrieve_player(map, line, err), 0);
	if (ft_strncmp(line, "L (", 3) == 0)
		return (retrieve_light(line + 3, err), 0);
	temp = is_end(line);
	if (temp == 1)
		return (1);
	if (temp == '1' && !tiles['1'])
		tiles['1'] = new_tile();
	if (temp == '1' && !tiles['1'])
		return (ft_perror(-1, "Internal error: tile creation", 0), *err = 1, 0);
	if (temp == '0' && !tiles['0'])
		tiles['0'] = new_tile();
	if (temp == '0' && !tiles['0'])
		return (ft_perror(-1, "Internal error: tile creation", 0), *err = 1, 0);
	lonely_type_switch(tiles, line, err, temp);
	return (0);
}
