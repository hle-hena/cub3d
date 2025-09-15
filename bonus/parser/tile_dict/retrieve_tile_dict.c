/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   retrieve_tile_dict.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 12:34:15 by hle-hena          #+#    #+#             */
/*   Updated: 2025/09/15 19:53:47 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D_bonus.h"

t_tile	**get_tile_dict(void)
{
	static t_tile	*tiles[256];

	return (tiles);
}

int	is_define(char *line, int *err)
{
	size_t	i;

	i = 1;
	while (ft_isspace(*(line + i)))
		i++;
	if (line[i] == '{' && line[0] == ' ')
		return (ft_perror(-1, "Trying to set ' ' to a non void tile.", 0),
			*err = 1);
	if (line[i] == '{' && ft_strchr("NSWE", line[0]))
	{
		printf("%s\n", line);
		return (ft_perror(-1, "Trying to set player_head_char to a tile.", 0),
			*err = 1);
	}
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
			return (NULL);
		line = ft_strtrim(temp, "\t\n ");
		if (!line)
			return (ft_perror(-1, "Internal error: malloc.", 0),
				ft_del((void **)&temp), *err = 1, NULL);
		if (!line[0])
		{
			(ft_del((void **)&line), ft_del((void **)&temp));
			continue ;
		}
		if (is_define(line, err))
			retrieve_tile(&get_tile_dict()[(int)*line], map_fd, line, err);
		else if (retrieve_lonely(map, line, err))
			return (ft_del((void **)&line), temp);
		(ft_del((void **)&line), ft_del((void **)&temp));
		if (*err)
			return (NULL);
	}
}
