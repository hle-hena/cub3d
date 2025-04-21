/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 16:17:06 by hle-hena          #+#    #+#             */
/*   Updated: 2025/04/21 19:51:10 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	remove_img(t_data *data, t_img texture)
{
	if (texture.img)
		mlx_destroy_image(data->mlx, texture.img);
}

int	clean_map(void)
{
	t_map	*map;
	t_data	*data;
	int		i;

	map = get_map();
	if (!map)
		return (0);
	data = get_data();
	i = -1;
	while (++i < 256)
	{
		if (map->tiles[i])
		{
			remove_img(data, map->tiles[i]->tex_no);
			remove_img(data, map->tiles[i]->tex_so);
			remove_img(data, map->tiles[i]->tex_ea);
			remove_img(data, map->tiles[i]->tex_we);
			remove_img(data, map->tiles[i]->tex_ce);
			remove_img(data, map->tiles[i]->tex_fl);
			ft_del((void **)&map->tiles[i]);
		}
	}
	if (map->matrix)
		ft_free_tab((void **)map->matrix, map->len);
	return (0);
}
