/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_data.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 10:18:57 by hle-hena          #+#    #+#             */
/*   Updated: 2025/06/10 15:01:53 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	close_threads(t_data *data)
{
	int	i;

	i = -1;
	while (++i < DRAW_THREADS)
	{
		pthread_mutex_lock(&data->thread_pool[i].mutex);
		data->thread_pool[i].ready = 2;
		pthread_cond_signal(&data->thread_pool[i].cond_start);
		pthread_mutex_unlock(&data->thread_pool[i].mutex);
		pthread_join(data->threads[i], NULL);
	}
}

int	clean_data(void)
{
	t_data	*data;

	data = get_data();
	clean_map();
	close_threads(data);
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
