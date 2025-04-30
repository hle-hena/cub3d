/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 22:06:06 by hle-hena          #+#    #+#             */
/*   Updated: 2025/04/30 18:42:23 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

	// struct timespec start, end;
	// clock_gettime(CLOCK_MONOTONIC, &start);
	// CODE TO BENCHMARK
	// clock_gettime(CLOCK_MONOTONIC, &end);
	// long ms = (end.tv_sec - start.tv_sec) * 1000000000L + (end.tv_nsec - start.tv_nsec);
	// printf("Took %ldns\n", ms);

static inline t_vec	reflect_across_mirror(t_vec point, t_hit *hit, t_point curr, int *bounce)
{
	float	offset;
	t_vec	final;

	*bounce = 0;
	while (++(*bounce) <= hit->bounces)
	{
		if (curr.y < hit->draw_start[*bounce - 1] || curr.y > hit->draw_end[*bounce - 1])
			return (point);
		if (hit->side[*bounce - 1] == 0)
		{
			offset = point.x - hit->hit[*bounce - 1].x;
			final.x = point.x - (2 * offset);
			final.y = point.y;
		}
		else
		{
			offset = point.y - hit->hit[*bounce - 1].y;
			final.x = point.x;
			final.y = point.y - (2 * offset);
		}
		point = final;
	}
	return (point);
}

static inline int	color_blend(int base_color, int light_color, float emittance)
{
	int		em;
	t_col	base;
	t_col	light;
	t_col	final;

	if (emittance < 0.01)
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

static inline void	draw_ceil(t_data *data, t_point curr, t_rdir ray, char *img, t_hit *hit)
{
	t_tile		*tile;
	t_flight	light;
	t_vec		cast;
	t_vec		world;
	t_point		pix;
	t_point		iworld;
	t_img		*tex;
	int			offset;
	int			bounce;

	cast = *(ray.cast_table + curr.y * data->win_wid + curr.x);
	world.x = data->map->player.x + cast.y * (ray.l.x + cast.x * ray.r.x);
	world.y = data->map->player.y + cast.y * (ray.l.y + cast.x * ray.r.y);
	world =  reflect_across_mirror(world, hit, curr, &bounce);
	iworld.x = (int)world.x;
	iworld.y = (int)world.y;
	if (world.y < 0 || world.y >= data->map->len || world.x < 0
		|| world.x >= data->map->wid)
		return (*(int *)img = 0, VOID);
	tile = *(ray.tile_dict + *(data->map->matrix + iworld.y * data->map->wid + iworld.x));
	if (!tile)
		return (*(int *)img = 0, VOID);
	tex = tile->tex_ce.img;
	pix.x = ((world.x - iworld.x) * tex->width);
	pix.y = ((world.y - iworld.y) * tex->height);
	if (pix.x >= tex->width)
		pix.x = tex->width - 1;
	if (pix.y >= tex->height)
		pix.y = tex->height - 1;
	offset = pix.y * tex->size_line + pix.x * tex->bpp;
	iworld.x = world.x * LMAP_PRECISION;
	iworld.y = world.y * LMAP_PRECISION;
	light = (data->lmap.lmap + iworld.x + iworld.y * data->lmap.wid)->ce_fl;
	*(int *)img = color_blend(*(int *)(tex->data + offset), light.color, light.emittance / bounce);
}

static inline void	draw_floor(t_data *data, t_point curr, t_rdir ray, char *img, t_hit *hit)
{
	t_tile		*tile;
	t_flight	light;
	t_vec		cast;
	t_vec		world;
	t_point		pix;
	t_point		iworld;
	t_img		*tex;
	int			offset;
	int			bounce;

	cast = *(ray.cast_table + curr.y * data->win_wid + curr.x);
	world.x = data->map->player.x + cast.y * (ray.l.x + cast.x * ray.r.x);
	world.y = data->map->player.y + cast.y * (ray.l.y + cast.x * ray.r.y);
	world =  reflect_across_mirror(world, hit, curr, &bounce);
	iworld.x = world.x;
	iworld.y = world.y;
	if (world.y < 0 || world.y >= data->map->len || world.x < 0
		|| world.x >= data->map->wid)
		return (*(int *)img = 0, VOID);
	tile = *(ray.tile_dict + *(data->map->matrix + iworld.y * data->map->wid + iworld.x));
	if (!tile)
		return (*(int *)img = 0, VOID);
	tex = tile->tex_fl.img;
	pix.x = ((world.x - iworld.x) * tex->width);
	pix.y = ((world.y - iworld.y) * tex->height);
	if (pix.x >= tex->width)
		pix.x = tex->width - 1;
	if (pix.y >= tex->height)
		pix.y = tex->height - 1;
	offset = pix.y * tex->size_line + pix.x * tex->bpp;
	iworld.x = world.x * LMAP_PRECISION;
	iworld.y = world.y * LMAP_PRECISION;
	light = (data->lmap.lmap + iworld.x + iworld.y * data->lmap.wid)->ce_fl;
	*(int *)img = color_blend(*(int *)(tex->data + offset), light.color, light.emittance / bounce);
}

static inline void	draw_wall(t_data *data, char *img, t_hit *hit)
{
	t_flight	light;
	t_point		light_point;

	light_point.x = (hit->hit[hit->bounces].x + (hit->ray_dir.x > 0 ? 0.001 : -0.001) * !hit->side[hit->bounces]) * LMAP_PRECISION;
	light_point.y = (hit->hit[hit->bounces].y + (hit->ray_dir.y > 0 ? 0.001 : -0.001) * hit->side[hit->bounces]) * LMAP_PRECISION;
	hit->tex_y = hit->tex_pos_fp >> 16;
	if (hit->side[hit->bounces] == 0)
		light = (data->lmap.lmap + light_point.x + light_point.y * data->lmap.wid)->no_so;
	else
		light = (data->lmap.lmap + light_point.x + light_point.y * data->lmap.wid)->we_ea;
	*(int *)img = color_blend(*(int *)(hit->tex_col + hit->tex_y * hit->texture->size_line), light.color, light.emittance / (hit->bounces + 1));
	hit->tex_pos_fp += hit->step_fp;
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
			if (curr.y >= hit->draw_start[hit->bounces] && curr.y < hit->draw_end[hit->bounces])
				draw_wall(data, img, hit);
			else if (curr.y < hit->draw_start[hit->bounces])
				draw_ceil(data, curr, td->ray_dir, img, hit);
			else if (curr.y >= hit->draw_end[hit->bounces])
				draw_floor(data, curr, td->ray_dir, img, hit);
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
		td[i].end_x = (i == DRAW_THREADS - 1) ? data->win_wid : (i + 1) * slice;
		td[i].add_next_line = (td[i].start_x + data->win_wid - td[i].end_x) * data->img.bpp;
		td[i].ray_dir = ray_dir;
		pthread_create(&threads[i], NULL, draw_walls_thread, &td[i]);
	}
	i = -1;
	while (++i < DRAW_THREADS)
		pthread_join(threads[i], NULL);
}

int	init_line_heights(t_data *data, t_hit *hit, t_vec ray_dir)
{
	int	i;
	int	tex_start;
	int	tex_end;
	int	line_height;

	i = -1;
	tex_end = 0;
	tex_start = 0;
	while (++i <= hit->bounces)
	{
		hit->dist[i] = hit->dist[i] * (ray_dir.x * data->cam.dir.x + ray_dir.y * data->cam.dir.y);
		if (hit->dist[i] <= 0.0f)
			hit->dist[i] = 0.0001f;
		line_height = (int)(data->win_len * 2 / hit->dist[i]) + 2;
		if (line_height == 0)
			line_height = 1;
		tex_start = -line_height / 2 + data->win_len / 2;
		tex_end = line_height / 2 + data->win_len / 2;
		hit->draw_start[i] = ft_max(tex_start, 0);
		hit->draw_end[i] = ft_min(tex_end, data->win_len);
	}
	return (tex_end - tex_start);
}


t_hit cast_ray(t_data *data, t_vec ray_dir)
{
	t_hit	hit;
	float	wall_x;
	int		line_height;

	hit = raycast(data, ray_dir, data->map->player);
	line_height = init_line_heights(data, &hit, ray_dir);
	if (line_height == 0)
		line_height = 1;
	if (hit.side[hit.bounces] == 0)
		wall_x = hit.hit[hit.bounces].y;
	else
		wall_x = hit.hit[hit.bounces].x;
	wall_x -= (int)wall_x;
	hit.step_fp = (hit.texture->height << 16) / line_height;
	hit.tex_pos_fp = (hit.draw_start[hit.bounces] - data->win_len / 2 + line_height / 2) * hit.step_fp;
	hit.tex_col = hit.texture->data + (int)(wall_x * hit.texture->width) * hit.texture->bpp;
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
	while (++x < data->win_wid)
	{
		cam_x = 2.0f * x / data->win_wid - 1.0f;
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
