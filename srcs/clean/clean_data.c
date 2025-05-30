/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_data.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 10:18:57 by hle-hena          #+#    #+#             */
/*   Updated: 2025/04/30 10:14:02 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	clean_data(void)
{
	t_data	*data;

	data = get_data();
	clean_map();
	// mlx_do_key_autorepeaton(data->mlx);
	if (data->img.img)
		mlx_destroy_image(data->mlx, data->img.img);
	if (data->win)
		mlx_destroy_window(data->mlx, data->win);
	if (data->mlx)
		mlx_destroy_display(data->mlx);
	if (data->hits)
		ft_del((void **)&data->hits);
	if (*get_cast_table())
		ft_del((void **)get_cast_table());
	if (data->lmap.lmap)
		ft_del((void **)&data->lmap.lmap);
	if (data->lmap.lights)
		ft_del((void **)&data->lmap.lights);
	ft_del((void **)&data->mlx);
	return (0);
}