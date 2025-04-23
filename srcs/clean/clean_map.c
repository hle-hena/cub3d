/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 16:17:06 by hle-hena          #+#    #+#             */
/*   Updated: 2025/04/23 13:54:38 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	remove_img(t_data *data, t_img *texture)
{
	if (texture)
	{
		if (texture->endian == 1)
			return ;
		if (texture->img)
			mlx_destroy_image(data->mlx, texture->img);
		ft_del((void **)&texture);
	}
}

void	clean_hash(t_data *data)
{
	t_img			**hash_table;
	unsigned int	i;

	i = -1;
	hash_table = get_hash_table();
	while (++i != HASH_SIZE)
	{
		if (!hash_table[i])
			continue;
		ft_del((void **)&hash_table[i]->path);
		hash_table[i]->endian = 0;
		remove_img(data, hash_table[i]);
	}
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
	clean_hash(data);
	if (map->matrix)
		ft_free_tab((void **)map->matrix, map->len);
	return (0);
}
