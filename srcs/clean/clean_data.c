/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_data.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 10:18:57 by hle-hena          #+#    #+#             */
/*   Updated: 2025/04/16 16:50:42 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	clear_stripes(t_data *data)
{
	int	i;
	int	nb_stripes;

	if (!data->img_stripes)
		return ;
	nb_stripes = ceil((float)data->win_wid / STRIPE_LEN);
	i = -1;
	while (++i < nb_stripes)
		ft_del((void **)&data->img_stripes[i].data);
	ft_del((void **)&data->img_stripes);
}

int	clean_data(void)
{
	t_data	*data;

	data = get_data();
	clean_map();
	// mlx_do_key_autorepeaton(data->mlx);
	clear_stripes(data);
	if (data->img.img)
		mlx_destroy_image(data->mlx, data->img.img);
	if (data->win)
		mlx_destroy_window(data->mlx, data->win);
	if (data->mlx)
		mlx_destroy_display(data->mlx);
	ft_del((void **)&data->mlx);
	return (0);
}