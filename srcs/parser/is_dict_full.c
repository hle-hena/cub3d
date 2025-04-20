/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_dict_full.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 21:54:54 by hle-hena          #+#    #+#             */
/*   Updated: 2025/04/20 22:11:09 by hle-hena         ###   ########.fr       */
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

int	is_dict_full(t_map *map)
{
	size_t	i;
	int		missing;

	i = -1;
	missing = 0;
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
	return (missing);
}
