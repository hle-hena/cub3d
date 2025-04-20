/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   retrieve_tile_dict.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 12:34:15 by hle-hena          #+#    #+#             */
/*   Updated: 2025/04/20 21:43:37 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	lonely_type_switch(t_map *map, char *line, int *err)
{
	char	*arg;

	if (!line[0])
		return (0);
	arg = ft_strtrim(line + 2, " \t\n"); //This is unsafe, what if line is < 2
	if (!arg)
		return (*err = 1, 0);
	if (!*arg)
		return (ft_perror(-1, ft_strsjoin((char *[]){"Empty line after \
identifier at ", line, ".", NULL}), 1), *err = 1, 0);
	if (ft_strncmp("NO ", line, 3) == 0)
		retrieve_texture(&(map->tiles['1'])->tex_no, arg, err, "NO");
	else if (ft_strncmp("SO ", line, 3) == 0)
		retrieve_texture(&(map->tiles['1'])->tex_so, arg, err, "SO");
	else if (ft_strncmp("EA ", line, 3) == 0)
		retrieve_texture(&(map->tiles['1'])->tex_ea, arg, err, "EA");
	else if (ft_strncmp("WE ", line, 3) == 0)
		retrieve_texture(&(map->tiles['1'])->tex_we, arg, err, "WE");
	else if (ft_strncmp("C ", line, 2) == 0)
		retrieve_texture(&(map->tiles['0'])->tex_ce, arg, err, "C");
	else if (ft_strncmp("F ", line, 2) == 0)
		retrieve_texture(&(map->tiles['0'])->tex_fl, arg, err, "F");
	else
		return (ft_del((void **)&arg), 1);
	return (ft_del((void **)&arg), 0);
}

int	retrieve_lonely(t_map *map, char *line, int *err)
{
	if (!map->tiles['1'])
		map->tiles['1'] = new_tile();
	if (!map->tiles['0'])
		map->tiles['0'] = new_tile();
	if (!map->tiles['1'] || !map->tiles['0'])
		return (ft_perror(-1, "Internal error: malloc.", 0), *err = 1, 0);
	return (lonely_type_switch(map, line, err));
}

int	is_define(char *line)
{
	size_t	i;

	i = 1;
	while (ft_isspace(*(line + i)))
		i++;
	return (line[i] == '{');
}

char	*retrieve_tile_dict(t_map *map, int map_fd, int *err)
{
	char	*temp;
	char	*line;

	while (1)
	{
		temp = get_next_line(map_fd);
		if (!temp)
			return (ft_perror(-1, "Internal error: malloc.", 0), *err = 1, NULL);
		line = ft_strtrim(temp, "\t\n ");
		ft_del((void **)&temp);
		if (!line)
			return (ft_perror(-1, "Internal error: malloc.", 0), *err = 1, NULL);
		if (!line[0])
		{
			ft_del((void **)&line);
			continue ;
		}
		if (is_define(line))
			retrieve_tile(map, map_fd, line, err);
		else if (retrieve_lonely(map, line, err))
				return (line);
		ft_del((void **)&line);
		if (*err)
			return (NULL);
	}
}
