/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 22:06:06 by hle-hena          #+#    #+#             */
/*   Updated: 2025/06/06 17:34:54 by hle-hena         ###   ########.fr       */
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


static inline t_col	color_blend(int base_color, int light_color, float emittance)
{
	int		em;
	t_col	base;
	t_col	light;
	t_col	final;

	if (emittance < 0.001)
		return ((t_col){0});
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
	return (final);
}

static inline void	set_pixels(t_data *data, char *img, int color)
{
	*(int *)(img) = color;
	*(int *)(img + data->img.bpp) = color;
	*(int *)(img + data->img.size_line) = color;
	*(int *)(img + data->img.size_line + data->img.bpp) = color;
}

#define FP_SHIFT 16
#define FP_ONE (1 << FP_SHIFT)
#define TO_FP(x) ((int)((x) * (float)(1 << FP_SHIFT) + 0.5f))
#define FROM_FP(x) ((float)x / FP_ONE)
#define MUL_FP(a, b) (((long long)(a) * (long long)(b)) >> FP_SHIFT)
#define DIV_FP(a, b) (((long long)(a) << FP_SHIFT) / (long long)(b))

static inline int to_linear(int c)
{
	int c_fp = DIV_FP(TO_FP(c), TO_FP(255.0f));
	return MUL_FP(c_fp, c_fp);
}

#define SRGB_TABLE_SIZE 1024
static short sqrt_fp_to_srgb[SRGB_TABLE_SIZE];

void init_sqrt_fp_to_srgb(void)
{
	for (int i = 0; i < SRGB_TABLE_SIZE; ++i)
	{
		float x = (float)i / (SRGB_TABLE_SIZE - 1);
		sqrt_fp_to_srgb[i] = (short)(sqrtf(x) * 255.0f + 0.5f);
	}
}

static inline int to_srgb(int fp_val)
{
	if (fp_val < 0)
		fp_val = 0;
	else if (fp_val >= FP_ONE)
		fp_val = FP_ONE - 1;
	int idx = (int)(((long long)fp_val * (SRGB_TABLE_SIZE - 1)) >> FP_SHIFT);
	return sqrt_fp_to_srgb[idx];
}

static inline t_col blend_color(t_col base_col, t_col reflected_col, int reflectance_fp)
{
	int inv_reflectance = (FP_ONE - reflectance_fp);
	inv_reflectance = MUL_FP(inv_reflectance, inv_reflectance);
	reflectance_fp = FP_ONE - inv_reflectance;
	int r1 = to_linear(base_col.re);
	int g1 = to_linear(base_col.gr);
	int b1 = to_linear(base_col.bl);
	int r2 = to_linear(reflected_col.re);
	int g2 = to_linear(reflected_col.gr);
	int b2 = to_linear(reflected_col.bl);
	int r = MUL_FP(r1, inv_reflectance) + MUL_FP(MUL_FP(r1, r2), reflectance_fp);
	int g = MUL_FP(g1, inv_reflectance) + MUL_FP(MUL_FP(g1, g2), reflectance_fp);
	int b = MUL_FP(b1, inv_reflectance) + MUL_FP(MUL_FP(b1, b2), reflectance_fp);
	t_col final = {
		to_srgb(r),
		to_srgb(g),
		to_srgb(b)
	};
	if (final.re > 255)
		final.re = 255;
	if (final.gr > 255)
		final.gr = 255;
	if (final.bl > 255)
		final.bl = 255;
	return (final);
}

static inline int	demenish_light(t_wpath *walls, int base_emittance, int max)
{
	int	i;

	i = -1;
	while (++i < max)
		base_emittance = MUL_FP(base_emittance, TO_FP(walls[i].reflectance));
	return (base_emittance);
}

static inline int	get_color(t_hit *hit, int nb_hit, t_col fall_back_color, int isnt_wall)
{
	t_col	final_color;
	t_col	prev_wall_color;
	t_col	wall_color;

	if (isnt_wall)
		prev_wall_color = fall_back_color;
	else
	{
		prev_wall_color = color_blend(*(int *)(hit->tex_col[nb_hit]
			+ (hit->tex_pos_fp[nb_hit] >> 16) * hit->texture[nb_hit]->size_line),
			hit->light[nb_hit]->color, FROM_FP(demenish_light(hit->wall, TO_FP(hit->light[nb_hit]->emittance), nb_hit)));
		hit->tex_pos_fp[nb_hit] += hit->step_fp[nb_hit];
	}
	final_color = prev_wall_color;
	while (--nb_hit >= 0)
	{
		wall_color = color_blend(*(int *)(hit->tex_col[nb_hit]
			+ (hit->tex_pos_fp[nb_hit] >> 16) * hit->texture[nb_hit]->size_line),
			hit->light[nb_hit]->color, FROM_FP(demenish_light(hit->wall, TO_FP(hit->light[nb_hit]->emittance), nb_hit)));
		final_color = blend_color(wall_color, prev_wall_color,
			TO_FP(hit->wall[nb_hit].reflectance));
		prev_wall_color = final_color;
		hit->tex_pos_fp[nb_hit] += hit->step_fp[nb_hit];
	}
	return ((final_color.re << 16) | (final_color.gr << 8) | final_color.bl);
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
	set_pixels(data, img, get_color(hit, bounce - 1, color_blend(*(int *)(tex->data + offset),
			light.color, FROM_FP(demenish_light(hit->wall, TO_FP(light.emittance), bounce - 1))), 1));
}

static inline void	draw_floor(t_data *data, t_point curr, t_rdir ray, char *img, t_hit *hit)
{
	t_flight	light;
	t_point		i_world;
	t_tile		*tile;
	t_vec		cast;
	t_vec		world;
	t_img		*tex;
	int			bounce;
	int			offset;

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
	tex = tile->tex_fl.img;
	offset = (int)((world.y - i_world.y) * tex->height) * tex->size_line
		+ (int)((world.x - i_world.x) * tex->width) * tex->bpp;
	light = data->lmap.lmap[(int)(world.x * LMAP_PRECISION)
		+ (int)(world.y * LMAP_PRECISION) * data->lmap.wid].ce_fl;
	set_pixels(data, img, get_color(hit, bounce - 1, color_blend(*(int *)(tex->data + offset),
			light.color, FROM_FP(demenish_light(hit->wall, TO_FP(light.emittance), bounce - 1))), 1));
}

static inline void	draw_wall(t_data *data, char *img, t_hit *hit)
{
	int			color;

	color = get_color(hit, hit->bounces, (t_col){0}, 0);
	set_pixels(data, img, color);
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
				draw_floor(data, curr, td->ray_dir, img, hit);
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
		hit->step_fp[i] = (hit->texture[i]->height << 16) / line_height;
		hit->tex_pos_fp[i] = (hit->draw_start[i]
			- data->render_h / 2 + line_height / 2) * hit->step_fp[i];
		hit->tex_col[i] = hit->texture[i]->data
			+ (int)(hit->wall[i].pos * hit->texture[i]->width) * hit->texture[i]->bpp;
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
