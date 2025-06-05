/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 22:06:06 by hle-hena          #+#    #+#             */
/*   Updated: 2025/06/05 16:22:03 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

	// struct timespec start, end;
	// clock_gettime(CLOCK_MONOTONIC, &start);
	// CODE TO BENCHMARK
	// clock_gettime(CLOCK_MONOTONIC, &end);
	// long ms = (end.tv_sec - start.tv_sec) * 1000000000L + (end.tv_nsec - start.tv_nsec);
	// printf("Took %ldns\n", ms);

static inline t_vec reflect_across_mirror(t_vec point, t_hit *hit, t_point curr, int *bounce)
{
	t_vec	normal;
	t_vec	to_point;
	t_vec	reflected;
	float	proj_len;

	*bounce = 0;
	while (++(*bounce) <= hit->bounces)
	{
		if (curr.y < hit->draw_start[*bounce - 1] || curr.y > hit->draw_end[*bounce - 1])
			return (point);

		normal = hit->wall[*bounce - 1].normal;
		to_point.x = point.x - hit->hit[*bounce - 1].x;
		to_point.y = point.y - hit->hit[*bounce - 1].y;
		proj_len = to_point.x * normal.x + to_point.y * normal.y;
		reflected.x = point.x - 2.0f * proj_len * normal.x;
		reflected.y = point.y - 2.0f * proj_len * normal.y;
		point = reflected;
	}
	return (point);
}


static inline int	color_blend(int base_color, int light_color, float emittance)
{
	int		em;
	t_col	base;
	t_col	light;
	t_col	final;

	if (emittance < 0.001)
		return (0);
	em = (int)(emittance * 256.0f);
	base = (t_col){(base_color >> 16) & 0xFF,
		(base_color >> 8) & 0xFF,
		base_color & 0xFF};
	light = (t_col){(light_color >> 16) & 0xFF,
		(light_color >> 8) & 0xFF,
		light_color & 0xFF};
	final = (t_col){(base.re * ((light.re * em) >> 8)) >> 8,
		(base.gr * ((light.gr * em) >> 8)) >> 8,
		(base.bl * ((light.bl * em) >> 8)) >> 8,
	};
	return ((final.re << 16) | (final.gr << 8) | final.bl);
}

static inline void	set_pixels(t_data *data, char *img, int color)
{
	*(int *)(img) = color;
	*(int *)(img + data->img.bpp) = color;
	*(int *)(img + data->img.size_line) = color;
	*(int *)(img + data->img.size_line + data->img.bpp) = color;
}

static inline float	demenish_light(t_wpath *walls, float base_emittance, int max)
{
	int	i;

	i = -1;
	while (++i < max)
		base_emittance *= walls[i].reflectance;
	return (base_emittance);
}

static inline void	draw_ceil(t_data *data, t_point curr, t_rdir ray, char *img, t_hit *hit)
{
	t_flight	light;
	t_point		i_world;
	t_tile		*tile;
	t_vec		cast;
	t_vec		world;
	t_img		*tex;
	int			bounce;
	int			offset;
	float		emittance;

	cast = ray.cast_table[curr.y * data->render_w + curr.x];
	world.x = data->map->player.x + cast.y * (ray.l.x + cast.x * ray.r.x);
	world.y = data->map->player.y + cast.y * (ray.l.y + cast.x * ray.r.y);
	world = reflect_across_mirror(world, hit, curr, &bounce);
	if (world.x < 0 || world.x >= data->map->wid || world.y < 0
		|| world.y >= data->map->len)
		return set_pixels(data, img, 0), VOID;
	i_world.x = (int)world.x;
	i_world.y = (int)world.y;
	tile = ray.tile_dict[data->map->matrix[i_world.y * data->map->wid + i_world.x]];
	if (!tile)
		return set_pixels(data, img, 0), VOID;
	tex = tile->tex_ce.img;
	offset = (int)((world.y - i_world.y) * tex->height) * tex->size_line
		+ (int)((world.x - i_world.x) * tex->width) * tex->bpp;
	light = data->lmap.lmap[(int)(world.x * LMAP_PRECISION)
		+ (int)(world.y * LMAP_PRECISION) * data->lmap.wid].ce_fl;
	emittance = demenish_light(hit->wall, light.emittance, bounce - 1);
	set_pixels(data, img,
		color_blend(*(int *)(tex->data + offset), light.color, emittance));
	tex = tile->tex_fl.img;
	set_pixels(data, data->img_end - curr.y * data->img.size_line * 2
		- (data->win_w - 1 - curr.x) * data->img.bpp * 2,
		color_blend(*(int *)(tex->data + offset), light.color, emittance));
}

static inline void	draw_wall(t_data *data, char *img, t_hit *hit)
{
	int			color;

	color = color_blend(*(int *)(hit->tex_col[hit->bounces]
		+ (hit->tex_pos_fp[hit->bounces] >> 16) * hit->texture->size_line),
		hit->light->color, demenish_light(hit->wall, hit->light->emittance, hit->bounces));
	set_pixels(data, img, color);
	hit->tex_pos_fp[hit->bounces] += hit->step_fp[hit->bounces];
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
	img = data->img.data + td->start_x * 2 * data->img.bpp;
	curr.y = -1;
	while (++curr.y < data->render_h)
	{
		curr.x = td->start_x - 1;
		while (++curr.x < td->end_x)
		{
			hit = &data->hits[curr.x];
			if (curr.y >= hit->draw_start[hit->bounces] && curr.y < hit->draw_end[hit->bounces])
				draw_wall(data, img, hit);
			else if (curr.y < hit->draw_start[hit->bounces])
				draw_ceil(data, curr, td->ray_dir, img, hit);
			else if (curr.y >= hit->draw_end[hit->bounces])
				;
			img += data->img.bpp * 2;
		}
		img += td->add_next_line + data->img.size_line;
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

	slice = data->render_w / DRAW_THREADS;
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
		td[i].end_x = (i == DRAW_THREADS - 1) ? data->render_w : (i + 1) * slice;
		td[i].add_next_line = (data->win_w + (td[i].start_x - td[i].end_x) * 2) * data->img.bpp;
		td[i].ray_dir = ray_dir;
		pthread_create(&threads[i], NULL, draw_walls_thread, &td[i]);
	}
	i = -1;
	while (++i < DRAW_THREADS)
		pthread_join(threads[i], NULL);
}

void	init_line_heights(t_data *data, t_hit *hit, int tex_start, int tex_end)
{
	int		i;
	int		line_height;

	i = -1;
	while (++i <= hit->bounces)
	{
		if (hit->dist[i] <= 0.0f)
			hit->dist[i] = 0.0001f;
		line_height = (int)(data->render_h * 2 / hit->dist[i]);
		if (line_height == 0)
			line_height = 1;
		tex_start = -line_height / 2 + data->render_h / 2;
		tex_end = line_height / 2 + data->render_h / 2;
		hit->draw_start[i] = ft_max(tex_start, 0);
		hit->draw_end[i] = ft_min(tex_end, data->render_h);
		line_height = tex_end - tex_start;
		if (line_height == 0)
			line_height = 1;
		hit->step_fp[i] = (hit->texture->height << 16) / line_height;
		hit->tex_pos_fp[i] = (hit->draw_start[i]
			- data->render_h / 2 + line_height / 2) * hit->step_fp[i];
		hit->tex_col[i] = hit->wall[i].texture.img->data
			+ (int)(hit->wall[i].pos * hit->texture->width) * hit->texture->bpp;
	}
}

t_hit cast_ray(t_data *data, t_vec ray_dir)
{
	t_hit	hit;

	hit = (t_hit){0};
	hit = raycast(data, ray_dir, (t_vec){data->map->player.x,
		data->map->player.y});
	init_line_heights(data, &hit, 0, 0);
	return (hit);
}

void cast_rays(t_data *data)
{
	int		x;
	float	cam_x;
	t_vec	ray_dir;
	float	look_dir;

	int		debug = 0;//////////////////////////set this
	struct	timespec start, end;
	long	ms;

	if (debug)
		clock_gettime(CLOCK_MONOTONIC, &start);
	look_dir = (float)data->map->player.rot * PI / 180;
	data->cam.dir = (t_vec){cos(look_dir), sin(look_dir)};
	data->cam.plane = (t_vec){-sin(look_dir) * tan((float)(60 * PI / 180) / 2),
		cos(look_dir) * tan((float)(60 * PI / 180) / 2)};
	x = -1;
	while (++x < data->render_w)
	{
		cam_x = 2.0f * x / data->render_w - 1.0f;
		ray_dir.x = data->cam.dir.x + data->cam.plane.x * cam_x;
		ray_dir.y = data->cam.dir.y + data->cam.plane.y * cam_x;
		data->hits[x] = cast_ray(data, ray_dir);
	}
	if (debug)
	{
		clock_gettime(CLOCK_MONOTONIC, &end);
		long ms = (end.tv_sec - start.tv_sec) * 1000000000L + (end.tv_nsec - start.tv_nsec);
		printf("Raycast took\t%ldns\n", ms);
	}


	if (debug)
		clock_gettime(CLOCK_MONOTONIC, &start);
	draw_walls(data);
	if (debug)
	{
		clock_gettime(CLOCK_MONOTONIC, &end);
		ms = (end.tv_sec - start.tv_sec) * 1000000000L + (end.tv_nsec - start.tv_nsec);
		printf("Draw took\t%ldns\n\n", ms);
	}
}
