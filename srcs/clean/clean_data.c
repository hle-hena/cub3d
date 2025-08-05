/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_data.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 10:18:57 by hle-hena          #+#    #+#             */
/*   Updated: 2025/08/04 10:32:56 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	close_threads(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->draw_thread)
	{
		pthread_mutex_lock(&data->thread_pool[i].mutex);
		data->thread_pool[i].ready = 2;
		pthread_cond_signal(&data->thread_pool[i].cond_start);
		pthread_mutex_unlock(&data->thread_pool[i].mutex);
		pthread_join(data->threads[i], NULL);
	}
	ft_del((void **)&data->threads);
	ft_del((void **)&data->thread_pool);
}

void	clean_mlx(t_data *data)
{
	int	i;

	i = -1;
	while (++i < IMG_BUFFER)
	{
		if (data->img[i].img)
			mlx_destroy_image(data->mlx, data->img[i].img);
	}
	if (data->empty)
	{
		if (data->empty->texture.img && data->empty->texture.img->img)
			mlx_destroy_image(data->mlx, data->empty->texture.img->img);
		ft_del((void **)&data->empty->flight);
		ft_del((void **)&data->empty->texture.img);
	}
	if (data->win)
		mlx_destroy_window(data->mlx, data->win);
	if (data->mlx)
		mlx_destroy_display(data->mlx);
	ft_del((void **)&data->empty);
}

void	clean_lmap(t_data *data)
{
	int	i;

	i = -1;
	ft_del((void **)&data->lmap.lights);
	while (++i < data->lmap.len * data->lmap.wid)
	{
		ft_lstclear(&data->lmap.lmap[i].flight, ft_del);
	}
}

int	clean_data(void)
{
	t_data	*data;

	data = get_data();
	clean_map();
	close_threads(data);
	mlx_do_key_autorepeaton(data->mlx);
	clean_mlx(data);
	clean_lmap(data);
	if (data->draw)
		ft_del((void **)&data->draw);
	if (*get_cast_table())
		ft_del((void **)get_cast_table());
	if (data->lmap.lmap)
		ft_del((void **)&data->lmap.lmap);
	if (data->lmap.lights)
		ft_del((void **)&data->lmap.lights);
	ft_del((void **)&data->mlx);
	return (0);
}
