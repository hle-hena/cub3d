/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   retrieve_player.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 19:33:55 by hle-hena          #+#    #+#             */
/*   Updated: 2025/04/23 14:17:52 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	retrieve_player(t_map *map, char *line, int *err)
{
	if (map->replace_tile != map->void_char)
		return (ft_perror(-1, "Trying to set the player replace tile twice.\
", 0), *err = 1, VOID);
	if (ft_strlen(line) > 3)
		return (ft_perror(-1, ft_strsjoin((char *[]){"The line '", line,"' \
should be P=[char to replace the player with]. Got too much char.", NULL}), 1),
		*err = 1, VOID);
	if (!line[2])
		return (ft_perror(-1, ft_strsjoin((char *[]){"The line '", line, "' is \
missing the char to replace the player with.", NULL}), 1), *err = 1, VOID);
	if (!get_tile_dict()[(int)line[2]])
		return (ft_perror(-1, "Trying to set the player replace tile to a tile \
not defined or not yet defined.\nThe tile should be defined before the 'P=' \
line.", 0), *err = 1, VOID);
	map->replace_tile = line[2];
}
