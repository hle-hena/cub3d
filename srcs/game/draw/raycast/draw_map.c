/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 22:06:06 by hle-hena          #+#    #+#             */
/*   Updated: 2025/06/10 10:04:14 by hle-hena         ###   ########.fr       */
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
	t_vec reflected = point;
	int b = 0;

	while (++b <= hit->bounces)
	{
		if (curr.y < hit->draw_start[b - 1] || curr.y > hit->draw_end[b - 1])
			break;

		t_vec normal = hit->wall[b - 1].normal;
		float dx = reflected.x - hit->hit[b - 1].x;
		float dy = reflected.y - hit->hit[b - 1].y;
		float dot = dx * normal.x + dy * normal.y;
		reflected.x -= 2.0f * dot * normal.x;
		reflected.y -= 2.0f * dot * normal.y;
	}
	*bounce = b;
	return reflected;
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


static inline short blend_channel(short base, short refl, int refl_fp)
{
	int inv = FP_ONE - refl_fp;
	int base_refl = (base * refl) >> 8;
	int blend = (base * inv + base_refl * refl_fp) >> FP_SHIFT;
	if (blend > 255)
		return (255);
	return (blend);
}


static inline t_col blend_color_fast(t_col base, t_col refl, int refl_fp)
{
	return (t_col){
		.re = blend_channel(base.re, refl.re, refl_fp),
		.gr = blend_channel(base.gr, refl.gr, refl_fp),
		.bl = blend_channel(base.bl, refl.bl, refl_fp),
	};
}

int	get_color(t_hit *hit, int nb_hit, t_col fall_back_color, int isnt_wall)
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
			hit->light[nb_hit]->color, hit->light[nb_hit]->emittance);
		hit->tex_pos_fp[nb_hit] += hit->step_fp[nb_hit];
	}
	final_color = prev_wall_color;
	while (--nb_hit >= 0)
	{
		wall_color = color_blend(*(int *)(hit->tex_col[nb_hit]
			+ (hit->tex_pos_fp[nb_hit] >> 16) * hit->texture[nb_hit]->size_line),
			hit->light[nb_hit]->color, hit->light[nb_hit]->emittance);
		final_color = blend_color_fast(wall_color, prev_wall_color,
			TO_FP(hit->wall[nb_hit].reflectance));
		prev_wall_color = final_color;
		hit->tex_pos_fp[nb_hit] += hit->step_fp[nb_hit];
	}
	return ((final_color.re << 16) | (final_color.gr << 8) | final_color.bl);
}

static inline void draw_ceil(t_data *data, t_point curr, t_rdir ray, char *img, t_hit *hit)
{
	int			bounce;
	int			offset;
	t_tile		*tile;
	t_img		*tex;
	t_col		color;
	t_flight	light;
	t_vec		cast;
	t_vec		world;
	int			ix;
	int			iy;

	cast = ray.cast_table[curr.y * data->render_w + curr.x];
	world = (t_vec){data->map->player.x + cast.y * (ray.l.x + cast.x * ray.r.x),
		data->map->player.y + cast.y * (ray.l.y + cast.x * ray.r.y)};
	world = reflect_across_mirror(world, hit, curr, &bounce);
	if (world.x < 0 || world.x >= data->map->wid || world.y < 0
		|| world.y >= data->map->len)
		return set_pixels(data, img, 0), VOID;
	ix = (int)world.x;
	iy = (int)world.y;
	tile = ray.tile_dict[data->map->matrix[iy * data->map->wid + ix]];
	if (!tile)
		return set_pixels(data, img, 0), VOID;
	tex = tile->tex_ce.img;
	offset = (int)((world.y - iy) * tex->height) * tex->size_line
		+ (int)((world.x - ix) * tex->width) * tex->bpp;
	light = data->lmap.lmap[(int)(world.x * LMAP_PRECISION)
		+ (int)(world.y * LMAP_PRECISION) * data->lmap.wid].ce_fl;
	int	*base_col = (int *)(tex->data + offset);
	color = color_blend(*base_col, light.color, light.emittance);
	set_pixels(data, img, get_color(hit, bounce - 1, color, 1));
}

static inline void draw_floor(t_data *data, t_point curr, t_rdir ray, char *img, t_hit *hit)
{
	int			bounce;
	int			offset;
	t_tile		*tile;
	t_img		*tex;
	t_col		color;
	t_flight	light;
	t_vec		cast;
	t_vec		world;
	int			ix;
	int			iy;

	cast = ray.cast_table[curr.y * data->render_w + curr.x];
	world = (t_vec){data->map->player.x + cast.y * (ray.l.x + cast.x * ray.r.x),
		data->map->player.y + cast.y * (ray.l.y + cast.x * ray.r.y)};
	world = reflect_across_mirror(world, hit, curr, &bounce);
	if (world.x < 0 || world.x >= data->map->wid || world.y < 0
		|| world.y >= data->map->len)
		return set_pixels(data, img, 0), VOID;
	ix = (int)world.x;
	iy = (int)world.y;
	tile = ray.tile_dict[data->map->matrix[iy * data->map->wid + ix]];
	if (!tile)
		return set_pixels(data, img, 0), VOID;
	tex = tile->tex_fl.img;
	offset = (int)((world.y - iy) * tex->height) * tex->size_line
		+ (int)((world.x - ix) * tex->width) * tex->bpp;
	light = data->lmap.lmap[(int)(world.x * LMAP_PRECISION)
		+ (int)(world.y * LMAP_PRECISION) * data->lmap.wid].ce_fl;
	int	*base_col = (int *)(tex->data + offset);
	color = color_blend(*base_col, light.color, light.emittance);
	set_pixels(data, img, get_color(hit, bounce - 1, color, 1));
}

static inline void	draw_wall(t_data *data, char *img, t_hit *hit)
{
	set_pixels(data, img, get_color(hit, hit->bounces, (t_col){0}, 0));
}

void	*draw_walls_section(t_th_draw *td)
{
	t_data		*data;
	t_hit		*hit;
	char		*img;
	t_point		curr;

	data = get_data();
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
		draw_walls_section(job);
		pthread_mutex_lock(&job->mutex);
		job->done = 1;
		pthread_cond_signal(&job->cond_done);
		pthread_mutex_unlock(&job->mutex);
	}
	return (NULL);
}

void	draw_walls(t_data *data)
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
	while (++i < DRAW_THREADS)
	{
		pthread_mutex_lock(&data->thread_pool[i].mutex);
		data->thread_pool[i].ray_dir = ray_dir;
		data->thread_pool[i].done = 0;
		data->thread_pool[i].ready = 1;
		pthread_cond_signal(&data->thread_pool[i].cond_start);
		pthread_mutex_unlock(&data->thread_pool[i].mutex);
	}
	i = -1;
	while (++i < DRAW_THREADS)
	{
		pthread_mutex_lock(&data->thread_pool[i].mutex);
		while (!data->thread_pool[i].done)
			pthread_cond_wait(&data->thread_pool[i].cond_done, &data->thread_pool[i].mutex);
		pthread_mutex_unlock(&data->thread_pool[i].mutex);
	}
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
