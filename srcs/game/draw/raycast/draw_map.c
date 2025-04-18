/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 22:06:06 by hle-hena          #+#    #+#             */
/*   Updated: 2025/04/18 21:31:19 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

	// struct timespec start, end;
	// clock_gettime(CLOCK_MONOTONIC, &start);
	// CODE TO BENCHMARK
	// clock_gettime(CLOCK_MONOTONIC, &end);
	// long ms = (end.tv_sec - start.tv_sec) * 1000000000L + (end.tv_nsec - start.tv_nsec);
	// printf("Took %ldns\n", ms);

void	*draw_walls_thread(void *arg)
{
	t_th_draw	*td;
	t_data		*data;
	char		*img;
	int			y;
	int			x;

	td = (t_th_draw *)arg;
	data = get_data();
	img = td->img.data;
	y = -1;
	while (++y < data->win_len)
	{
		x = -1;
		while (++x < td->end_x)
		{
			if (y >= data->hits[x + td->offset_x].draw_start && y < data->hits[x + td->offset_x].draw_end)
			{
				data->hits[x + td->offset_x].tex_y = data->hits[x + td->offset_x].tex_pos_fp >> 16;
				*(int *)img = *(int *)(data->hits[x + td->offset_x].tex_col + data->hits[x + td->offset_x].tex_y * data->hits[x + td->offset_x].texture.size_line);
				data->hits[x + td->offset_x].tex_pos_fp += data->hits[x + td->offset_x].step_fp;
			}
			img += td->img.bpp;
		}
	}
	return (NULL);
}

void	draw_walls(t_data *data)
{
	pthread_t	threads[DRAW_THREADS];
	t_th_draw	td[DRAW_THREADS];
	int			i;

	i = -1;
	while (++i < DRAW_THREADS)
	{
		td[i].img = data->th_img[i];
		td[i].end_x = data->th_img[i].size_line / data->th_img[i].bpp;
		td[i].offset_x = i * data->win_wid / DRAW_THREADS;
		pthread_create(&threads[i], NULL, draw_walls_thread, &td[i]);
	}
	i = -1;
	while (++i < DRAW_THREADS)
	{
		pthread_join(threads[i], NULL);
		mlx_put_image_to_window(data->mlx, data->win, data->th_img[i].img, i * data->win_wid / DRAW_THREADS, 0);
	}
}

t_hit cast_ray(t_data *data, t_vec ray_dir)
{
	t_hit	ray_hit;
	float	wall_x;
	int		line_height;
	int		tex_start;
	int		tex_end;

	ray_hit = raycast(data, ray_dir, data->map->player);
	ray_hit.ray_dir = ray_dir;
	ray_hit.dist = ray_hit.dist * (ray_dir.x * data->cam.dir.x + ray_dir.y * data->cam.dir.y);
	if (ray_hit.dist <= 0.0f)
		ray_hit.dist = 0.0001f;
	line_height = (int)(data->win_len / ray_hit.dist);
	if (line_height == 0)
		line_height = 1;
	tex_start = -line_height / 2 + data->win_len / 2;
	tex_end = line_height / 2 + data->win_len / 2;
	ray_hit.draw_start = ft_max(tex_start, 0);
	ray_hit.draw_end = ft_min(tex_end, data->win_len - 1);
	if (ray_hit.side == 0)
		wall_x = ray_hit.ray_hit.y;
	else
		wall_x = ray_hit.ray_hit.x;
	wall_x -= (int)wall_x;
	line_height = tex_end - tex_start;
	ray_hit.step_fp = (ray_hit.texture.height << 16) / line_height;
	ray_hit.tex_pos_fp = (ray_hit.draw_start - data->win_len / 2 + line_height / 2) * ray_hit.step_fp;
	ray_hit.tex_col = ray_hit.texture.data + (int)(wall_x * ray_hit.texture.width) * ray_hit.texture.bpp;
	return (ray_hit);
}

void cast_rays(t_data *data)
{
	int		x;
	float	cam_x;
	t_vec	ray_dir;
	float	look_dir;

	look_dir = (float)data->map->player.rot * PI / 180;
	data->cam.dir = (t_vec){cos(look_dir), sin(look_dir), 0};
	data->cam.plane = (t_vec){-sin(look_dir) * tan((float)(60 * PI / 180) / 2),
		cos(look_dir) * tan((float)(60 * PI / 180) / 2), 0};
	x = -1;
	while (++x < data->win_wid)
	{
		cam_x = 2.0f * x / data->win_wid - 1.0f;
		ray_dir.x = data->cam.dir.x + data->cam.plane.x * cam_x;
		ray_dir.y = data->cam.dir.y + data->cam.plane.y * cam_x;
		data->hits[x] = cast_ray(data, ray_dir);
	}
	draw_walls(data);
}
