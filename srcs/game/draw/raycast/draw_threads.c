/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_threads.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 11:40:41 by hle-hena          #+#    #+#             */
/*   Updated: 2025/08/28 10:11:10 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void	*draw_walls_thread(void *arg)
{
	t_th_draw	*job;

	job = (t_th_draw *)arg;
	while (1)
	{
		pthread_mutex_lock(&job->mutex);
		while (!job->ready)
			pthread_cond_wait(&job->cond_start, &job->mutex);
		if (job->ready == 2)
			break ;
		job->ready = 0;
		pthread_mutex_unlock(&job->mutex);
		draw_walls(job);
		pthread_mutex_lock(&job->mutex);
		job->done = 1;
		pthread_cond_signal(&job->cond_done);
		pthread_mutex_unlock(&job->mutex);
	}
	return (NULL);
}

void	wait_threads(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->draw_thread)
	{
		pthread_mutex_lock(&data->thread_pool[i].mutex);
		while (!data->thread_pool[i].done)
			pthread_cond_wait(&data->thread_pool[i].cond_done,
				&data->thread_pool[i].mutex);
		pthread_mutex_unlock(&data->thread_pool[i].mutex);
	}
}

void	start_threads(t_data *data)
{
	t_rdir	ray_dir;
	int		i;

	ray_dir.l.x = data->cam.dir.x - data->cam.plane.x;
	ray_dir.l.y = data->cam.dir.y - data->cam.plane.y;
	ray_dir.r.x = data->cam.plane.x * 2;
	ray_dir.r.y = data->cam.plane.y * 2;
	ray_dir.cast_table = *get_cast_table();
	ray_dir.tile_dict = get_tile_dict();
	i = -1;
	while (++i < data->draw_thread)
	{
		pthread_mutex_lock(&data->thread_pool[i].mutex);
		data->thread_pool[i].ray_dir = ray_dir;
		data->thread_pool[i].info = (t_simd){0};
		data->thread_pool[i].info.utils = &data->simd_utils;
		data->thread_pool[i].done = 0;
		data->thread_pool[i].ready = 1;
		pthread_cond_signal(&data->thread_pool[i].cond_start);
		pthread_mutex_unlock(&data->thread_pool[i].mutex);
	}
	wait_threads(data);
}
