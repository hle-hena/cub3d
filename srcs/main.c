/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 16:05:37 by hle-hena          #+#    #+#             */
/*   Updated: 2025/06/09 14:58:06 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

t_data	*get_data(void)
{
	static t_data	data = (t_data){0};

	return (&data);
}

void	init_mlx(t_data *data)
{
	data->empty = ft_calloc(1, sizeof(t_void));
	data->empty->flight = ft_calloc(1, sizeof(t_flight));
	data->empty->texture.img = malloc(sizeof(t_img));
	data->empty->texture.img = mlx_new_image(data->mlx, 1, 1);
	data->empty->texture.img->data = mlx_get_data_addr(data->empty->texture.img,
		&data->empty->texture.img->bpp, &data->empty->texture.img->size_line,
		&data->empty->texture.img->endian);
	data->empty->texture.img->bpp /= 8;
	mlx_get_screen_size(data->mlx, &data->render_w, &data->render_h);
	data->render_h *= 0.9;
	// data->render_h *= 2;
	// data->render_w *= 2;
	data->render_h += (data->render_h % 2);
	data->render_w += (data->render_w % 2);
	data->win_h = data->render_h;
	data->win_w = data->render_w;
	data->render_h /= 2;
	data->render_w /= 2;
	data->win = mlx_new_window(data->mlx, data->win_w, data->win_h,
		"Cub3d");
	data->img.img = mlx_new_image(data->mlx, data->win_w, data->win_h);
	data->img.data = mlx_get_data_addr(data->img.img, &data->img.bpp,
		&data->img.size_line, &data->img.endian);
	data->img.bpp /= 8;
	data->img_end = data->img.data + (data->win_h - 1) * data->img.size_line
		+ (data->win_w - 1) * data->img.bpp;
	data->event = (t_event){0};
	data->delta_t = 0;
	data->lmap.nb_ls = 0;
	data->lmap.lights = NULL;
}

void	init_utils(t_data *data)
{
	int	err;

	err = 0;
	data->map->mini_map = (t_point)
		{data->win_w * 0.9, data->win_h * 0.15};
	data->map->mini_map_scale = 32;
	data->hits = malloc(data->render_w * sizeof(t_hit));
	if (!data->hits)
		ft_perror(1, "cub3d: Internal error: malloc.", clean_data());
	fill_cast_table(data, &err);
	if (err)
		ft_perror(1, NULL, 0);
}

void init_draw_threads(t_data *data)
{
	int	i;
	int slice;

	slice = data->render_w / DRAW_THREADS;
	i = -1;
	while (++i < DRAW_THREADS)
	{
		data->thread_pool[i].start_x = i * slice;
		data->thread_pool[i].end_x = (i == DRAW_THREADS - 1) ? data->render_w : (i + 1) * slice;
		data->thread_pool[i].add_next_line = (data->win_w +
			(data->thread_pool[i].start_x - data->thread_pool[i].end_x) * 2) * data->img.bpp;
		data->thread_pool[i].ready = 0;
		data->thread_pool[i].done = 0;
		pthread_mutex_init(&data->thread_pool[i].mutex, NULL);
		pthread_cond_init(&data->thread_pool[i].cond_start, NULL);
		pthread_cond_init(&data->thread_pool[i].cond_done, NULL);
		pthread_create(&data->threads[i], NULL, draw_walls_thread, &data->thread_pool[i]);
	}
}

int	main(int ac, char **av)
{
	t_data	*data;

	data = get_data();
	data->mlx = mlx_init();
	mlx_do_key_autorepeatoff(data->mlx);
	data->map = load_map(ac, av);
	if (data->map)
	{
		if (create_lmap(data))
		{
			clean_data();
			return (0);
		}
		// print_dict(data);
		init_mlx(data);
		init_utils(data);
		init_draw_threads(data);
		loop();
	}
	clean_data();
	return (0);
}
