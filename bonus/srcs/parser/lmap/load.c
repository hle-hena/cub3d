/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 12:08:29 by hle-hena          #+#    #+#             */
/*   Updated: 2025/08/28 10:11:10 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void	print_progress(float progress, int index, int nb_clear, int width)
{
	int	i;

	if (nb_clear)
		clear_n_lines(nb_clear);
	if (index == 0)
		printf("\001\033[94m\002Light map\t[");
	else
		printf("\001\033[34m\002    Light %3d\t[", index);
	i = -1;
	while (++i < width)
	{
		if (i < progress)
			printf("=");
		else
			printf(" ");
	}
	printf("] %3.0f%%\n\001\033[0m", progress * 100 / width);
}

void	raytrace_source(t_data *data, t_light light)
{
	float	deg;
	t_vec	dir;

	deg = 0;
	print_progress(40.0f * (float)(light.index - 1) / data->lmap.nb_ls,
		0, 2 * (light.index != 1), 40);
	print_progress((deg / 360.0f) * 30, light.index, 0, 30);
	while (deg < 360)
	{
		dir.x = cos(deg * PI / 180);
		dir.y = sin(deg * PI / 180);
		raytrace(data, light, dir);
		print_progress(30.0f * (deg / 360.0f), light.index, 1, 30);
		deg += 0.01;
	}
	print_progress(40.0f * (float)(light.index) / data->lmap.nb_ls,
		0, 2, 40);
	print_progress(30, light.index, 0, 30);
}

int	create_lmap(t_data *data)
{
	int			i;
	t_lmap		*lmap;

	lmap = &data->lmap;
	lmap->wid = data->map->wid * LMAP_PRECISION;
	lmap->len = data->map->len * LMAP_PRECISION;
	data->lmap.lmap = ft_calloc(lmap->wid * lmap->len, sizeof(t_tlight));
	if (!lmap->lmap)
		return (ft_perror(-1, "The light map was too big and the malloc didn't \
go through.", 0), 1);
	if (!lmap->lights)
		return (0);
	i = -1;
	while (++i < lmap->nb_ls)
	{
		lmap->lights[i].index = i + 1;
		raytrace_source(data, lmap->lights[i]);
	}
	printf("\n\n");
	return (0);
}
