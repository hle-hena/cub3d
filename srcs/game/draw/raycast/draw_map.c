/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 22:06:06 by hle-hena          #+#    #+#             */
/*   Updated: 2025/08/28 15:29:31 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static inline void	draw_ceil(t_data *data, t_point curr, t_rdir ray, char *img)
{
	t_img		*tex;
	t_vec		cast;
	t_vec		world;
	t_point		i;

	cast = ray.cast_table[curr.y * data->win_wid + curr.x];
	world = (t_vec){data->map->player.x + cast.y * (ray.l.x + cast.x
			* ray.r.x), data->map->player.y + cast.y
		* (ray.l.y + cast.x * ray.r.y)};
	if (world.x < 0 || world.x >= data->map->wid || world.y < 0
		|| world.y >= data->map->len)
		return ((void)(*(int *)img = 0));
	i = (t_point){(int)world.x, (int)world.y};
	if (!ray.tile_dict[data->map->matrix[i.y * data->map->wid + i.x]])
		return ((void)(*(int *)img = 0));
	tex = ray.tile_dict[data->map->matrix[i.y * data->map->wid + i.x]]->tex_ce;
	*(int *)img = *(((int *)tex->data) + (tex->size_line / 4)
			* (int)((world.y - i.y) * tex->height) + (int)((world.x - i.x)
				* tex->width));
}

static inline void	draw_floor(t_data *data, t_point curr, t_rdir ray,
	char *img)
{
	t_img		*tex;
	t_vec		cast;
	t_vec		world;
	t_point		i;

	cast = ray.cast_table[curr.y * data->win_wid + curr.x];
	world = (t_vec){data->map->player.x + cast.y * (ray.l.x + cast.x
			* ray.r.x), data->map->player.y + cast.y
		* (ray.l.y + cast.x * ray.r.y)};
	if (world.x < 0 || world.x >= data->map->wid || world.y < 0
		|| world.y >= data->map->len)
		return ((void)(*(int *)img = 0));
	i = (t_point){(int)world.x, (int)world.y};
	if (!ray.tile_dict[data->map->matrix[i.y * data->map->wid + i.x]])
		return ((void)(*(int *)img = 0));
	tex = ray.tile_dict[data->map->matrix[i.y * data->map->wid + i.x]]->tex_fl;
	*(int *)img = *(((int *)tex->data) + (tex->size_line / 4)
			* (int)((world.y - i.y) * tex->height) + (int)((world.x - i.x)
				* tex->width));
}

void	*draw_walls_thread(void *arg)
{
	t_th_draw	*td;
	t_data		*data;
	t_hit		*hit;
	char		*img;
	t_point		curr;

	td = (t_th_draw *)arg;
	data = td->data;
	img = data->img.data + td->start_x * data->img.bpp;
	curr.y = -1;
	while (++curr.y < data->win_len)
	{
		curr.x = td->start_x - 1;
		while (++curr.x < td->end_x)
		{
			hit = &data->hits[curr.x];
			if (curr.y >= hit->draw_start && curr.y < hit->draw_end)
			{
				hit->tex_y = hit->tex_pos_fp >> 16;
				*(int *)img = *(int *)(hit->tex_col + hit->tex_y
						* hit->texture->size_line);
				hit->tex_pos_fp += hit->step_fp;
			}
			else if (curr.y < hit->draw_start)
				draw_ceil(data, curr, td->ray_dir, img);
			else
				draw_floor(data, curr, td->ray_dir, img);
			img += data->img.bpp;
		}
		img += td->add_next_line;
	}
	return (NULL);
}

void	draw_walls(t_data *data)
{
	pthread_t	threads[DRAW_THREADS];
	t_th_draw	td[DRAW_THREADS];
	t_rdir		ray_dir;
	int			slice;
	int			i;

	slice = data->win_wid / DRAW_THREADS;
	i = -1;
	ray_dir.l.x = data->cam.dir.x - data->cam.plane.x;
	ray_dir.l.y = data->cam.dir.y - data->cam.plane.y;
	ray_dir.r.x = data->cam.plane.x * 2;
	ray_dir.r.y = data->cam.plane.y * 2;
	ray_dir.cast_table = *get_cast_table();
	ray_dir.tile_dict = get_tile_dict();
	while (++i < DRAW_THREADS)
	{
		td[i].data = data;
		td[i].start_x = i * slice;
		td[i].end_x = (i + 1) * slice;
		if (i == DRAW_THREADS - 1)
			td[i].end_x = data->win_wid;
		td[i].add_next_line = (td[i].start_x + data->win_wid - td[i].end_x)
			* data->img.bpp;
		td[i].ray_dir = ray_dir;
		pthread_create(&threads[i], NULL, draw_walls_thread, &td[i]);
	}
	i = -1;
	while (++i < DRAW_THREADS)
		pthread_join(threads[i], NULL);
}

t_hit	cast_ray(t_data *data, t_vec ray_dir)
{
	t_hit	ray_hit;
	float	wall_x;
	int		line_height;
	int		tex_start;
	int		tex_end;

	ray_hit = raycast(data, ray_dir, data->map->player);
	ray_hit.ray_dir = ray_dir;
	ray_hit.dist = ray_hit.dist * (ray_dir.x * data->cam.dir.x + ray_dir.y
			* data->cam.dir.y);
	if (ray_hit.dist <= 0.0f)
		ray_hit.dist = 0.0001f;
	line_height = (int)(data->win_len * 2 / ray_hit.dist);
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
	ray_hit.step_fp = (ray_hit.texture->height << 16) / line_height;
	ray_hit.tex_pos_fp = (ray_hit.draw_start - data->win_len / 2 + line_height
			/ 2) * ray_hit.step_fp;
	ray_hit.tex_col = ray_hit.texture->data + (int)(wall_x
			* ray_hit.texture->width) * ray_hit.texture->bpp;
	return (ray_hit);
}

void	cast_rays(t_data *data)
{
	int		x;
	float	cam_x;
	t_vec	ray_dir;
	float	look_dir;

	look_dir = (float)data->map->player.rot * PI / 180;
	data->cam.dir = (t_vec){cos(look_dir), sin(look_dir)};
	data->cam.plane = (t_vec){-sin(look_dir) * tan((float)(60 * PI / 180) / 2),
		cos(look_dir) * tan((float)(60 * PI / 180) / 2)};
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
