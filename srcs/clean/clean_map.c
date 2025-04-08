/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 16:17:06 by hle-hena          #+#    #+#             */
/*   Updated: 2025/04/08 10:40:49 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	clean_map(void)
{
	t_map	*map;
	t_data	*data;

	map = get_map();
	data = get_data();
	if (map->img_ea.img)
		mlx_destroy_image(data->mlx, map->img_ea.img);
	if (map->img_we.img) 
		mlx_destroy_image(data->mlx, map->img_we.img);
	if (map->img_so.img) 
		mlx_destroy_image(data->mlx, map->img_so.img);
	if (map->img_no.img) 
		mlx_destroy_image(data->mlx, map->img_no.img);
	ft_free_tab((void **)map->matrix, map->len);
	return (0);
}
