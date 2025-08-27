/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 16:05:37 by hle-hena          #+#    #+#             */
/*   Updated: 2025/08/27 17:28:50 by gakarbou         ###   ########.fr       */
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
	int	i;

	data->empty = ft_calloc(1, sizeof(t_void));
	if (!data->empty)
		ft_perror(1, "Internal error: malloc", clean_data());
	data->empty->flight = ft_calloc(1, sizeof(t_flight));
	data->empty->texture.img = ft_calloc(1, sizeof(t_img));
	if (!data->empty->flight || !data->empty->texture.img)
		ft_perror(1, "Internal error: malloc", clean_data());
	new_image(data, data->empty->texture.img, 1, 1);
	mlx_get_screen_size(data->mlx, &data->render_w, &data->render_h);
	data->render_h = (data->render_h * 0.9) + ((int)(data->render_h * 0.9) % 2);
	data->render_w += (data->render_w % 2);
	data->win_h = data->render_h;
	data->win_w = data->render_w;
	data->render_h /= 2;
	data->render_w /= 2;
	data->win = mlx_new_window(data->mlx, data->win_w, data->win_h,
			"Cub3d");
	i = -1;
	while (++i < IMG_BUFFER)
		new_image(data, &data->img[i], data->win_w, data->win_h);
	data->event = (t_event){0};
	data->delta_t = 0;
	data->img_buffer = 0;
}

void	init_utils(t_data *data)
{
	int	err;

	err = 0;
	data->map->mini_map = (t_point){data->win_w * 0.9, data->win_h * 0.15};
	data->map->mini_map_scale = 32;
	data->draw = malloc(data->render_w * sizeof(t_draw));
	if (!data->draw)
		ft_perror(1, "cub3D: Internal error: malloc.", clean_data());
	fill_cast_table(data, &err);
	if (err)
		ft_perror(1, NULL, 0);
	data->simd_utils.div = _mm256_set1_ps(1.0f / 255);
	data->simd_utils.mul = _mm256_set1_ps(255);
	data->simd_utils.one = _mm256_set1_ps(1);
}

void	init_draw_threads(t_data *data)
{
	int	i;
	int	slice;

	data->draw_thread = sysconf(_SC_NPROCESSORS_ONLN);
	data->thread_pool = malloc(data->draw_thread * sizeof(t_th_draw));
	data->threads = malloc(data->draw_thread * sizeof(pthread_t));
	slice = data->render_w / data->draw_thread;
	i = -1;
	while (++i < data->draw_thread)
	{
		data->thread_pool[i].start_x = i * slice;
		data->thread_pool[i].end_x = (i + 1) * slice;
		if (i == data->draw_thread - 1)
			data->thread_pool[i].end_x = data->render_w;
		data->thread_pool[i].add_next_line = (data->win_w
				+ (data->thread_pool[i].start_x - data->thread_pool[i].end_x)
				* 2);
		data->thread_pool[i].ready = 0;
		data->thread_pool[i].done = 0;
		pthread_mutex_init(&data->thread_pool[i].mutex, NULL);
		pthread_cond_init(&data->thread_pool[i].cond_start, NULL);
		pthread_cond_init(&data->thread_pool[i].cond_done, NULL);
		pthread_create(&data->threads[i], NULL, draw_walls_thread,
			&data->thread_pool[i]);
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
		init_mlx(data);
		init_utils(data);
		init_draw_threads(data);
		loop();
	}
	clean_data();
	return (0);
}
