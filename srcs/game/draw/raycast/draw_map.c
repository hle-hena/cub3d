/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 22:06:06 by hle-hena          #+#    #+#             */
/*   Updated: 2025/06/13 11:34:49 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

	// struct timespec start, end;
	// clock_gettime(CLOCK_MONOTONIC, &start);
	// CODE TO BENCHMARK
	// clock_gettime(CLOCK_MONOTONIC, &end);
	// long ms = (end.tv_sec - start.tv_sec) * 1000000000L + (end.tv_nsec - start.tv_nsec);
	// printf("Took %ldns\n", ms);

static inline t_vec reflect_across_mirror(t_vec point, t_draw *draw, t_point curr, int *bounce)
{
	t_vec reflected = point;
	int b = 0;

	while (++b <= draw->bounces)
	{
		if (curr.y < draw->draw_start[b - 1] || curr.y > draw->draw_end[b - 1])
			break;

		t_vec normal = draw->normal[b-1];
		float dx = reflected.x - draw->hit[b - 1].x;
		float dy = reflected.y - draw->hit[b - 1].y;
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

static inline void	set_pixels(t_data *data, int *img, int color)
{
	*(img) = color;
	*(img + 1) = color;
	*(img + data->img.size_line) = color;
	*(img + data->img.size_line + 1) = color;
}

static inline __m256	color_blend_simd(__m256 base_color, __m256 light_color, __m256 emittance, __m256 inv)
{
	return (_mm256_mul_ps(_mm256_mul_ps(
		_mm256_mul_ps(base_color, light_color),
		emittance
	), inv));
}

static inline __m256 blend_channel_simd(__m256 base, __m256 other, __m256 refl, __m256 inv_refl)
{
	return (_mm256_mul_ps(
		base,
		_mm256_add_ps(inv_refl, _mm256_mul_ps(_mm256_div_ps(other, _mm256_set1_ps(255.0f)), refl))
	));
}

void get_colors_simd(t_simd info, int out_colors[8])
{
	__m256	prev_r;
	__m256	prev_g;
	__m256	prev_b;
	__m256	curr_r;
	__m256	curr_g;
	__m256	curr_b;
	__m256	refl;
	__m256	inv_refl;
	__m256	one;
	__m256	max;
	__m256	inv;
	__m256	mask;
	__m256	nb_hit;
	__m256	curr_hit;
	int		i;
	int		max_hit;
	max_hit = 0;
	i = -1;
	while (++i < 8)
	{
		if (info.nb_hit[i] > max_hit)
			max_hit = info.nb_hit[i];
	}
	one = _mm256_set1_ps(1);
	max = _mm256_set1_ps(256);
	inv = _mm256_set1_ps(1.0f / 65536.0f);
	nb_hit = _mm256_cvtepi32_ps(_mm256_load_si256((__m256i *)info.nb_hit));
	prev_r = _mm256_load_ps(info.fallback.r);
	prev_g = _mm256_load_ps(info.fallback.g);
	prev_b = _mm256_load_ps(info.fallback.b);
	while (--max_hit >= 0)
	{
		__m256	em = _mm256_mul_ps(_mm256_load_ps(info.emittance[max_hit]), max);
		curr_r = color_blend_simd(_mm256_load_ps(info.textures[max_hit].r),
			_mm256_load_ps(info.light_color[max_hit].r), em, inv);
		curr_g = color_blend_simd(_mm256_load_ps(info.textures[max_hit].g),
			_mm256_load_ps(info.light_color[max_hit].g), em, inv);
		curr_b = color_blend_simd(_mm256_load_ps(info.textures[max_hit].b),
			_mm256_load_ps(info.light_color[max_hit].b), em, inv);
		refl = _mm256_load_ps(info.refl_val[max_hit]);
		inv_refl = _mm256_sub_ps(one, refl);
		curr_hit = _mm256_set1_ps(max_hit);
		mask = _mm256_cmp_ps(curr_hit, nb_hit, _CMP_LT_OQ);
		prev_r = _mm256_blendv_ps(prev_r,
			blend_channel_simd(curr_r, prev_r, refl, inv_refl), mask);
		prev_g = _mm256_blendv_ps(prev_g,
			blend_channel_simd(curr_g, prev_g, refl, inv_refl), mask);
		prev_b = _mm256_blendv_ps(prev_b,
			blend_channel_simd(curr_b, prev_b, refl, inv_refl), mask);
	}
	__m256i r_int = _mm256_cvtps_epi32(prev_r);
	__m256i g_int = _mm256_cvtps_epi32(prev_g);
	__m256i b_int = _mm256_cvtps_epi32(prev_b);
	__m256i packed = _mm256_or_si256(
		_mm256_or_si256(
			_mm256_slli_epi32(r_int, 16),
			_mm256_slli_epi32(g_int, 8)
		),
		b_int
	);
	_mm256_store_si256((__m256i *)out_colors, packed);
}

//This function is cache-miss-read heavy ...
static inline void	setup_color(t_draw *draw, t_th_draw *td, t_col fallback, int nb_hit)
{
	int	col;

	td->info.fallback.r[td->current_pix] = fallback.re;
	td->info.fallback.g[td->current_pix] = fallback.gr;
	td->info.fallback.b[td->current_pix] = fallback.bl;
	td->info.nb_hit[td->current_pix] = nb_hit;
	while (--nb_hit >= 0)
	{
		col = *(draw->tex_col[nb_hit]//data miss read here
			+ (draw->tex_pos_fp[nb_hit] >> 16) * draw->tex_sizeline[nb_hit]);//data miss read here
		td->info.textures[nb_hit].r[td->current_pix] = (col >> 16) & 0xFF;
		td->info.textures[nb_hit].g[td->current_pix] = (col >> 8) & 0xFF;
		td->info.textures[nb_hit].b[td->current_pix] = col & 0xFF;
		col = draw->light_color[nb_hit];//data miss read here
		td->info.light_color[nb_hit].r[td->current_pix] = (col >> 16) & 0xFF;
		td->info.light_color[nb_hit].g[td->current_pix] = (col >> 8) & 0xFF;
		td->info.light_color[nb_hit].b[td->current_pix] = col & 0xFF;
		td->info.refl_val[nb_hit][td->current_pix] = draw->reflectance[nb_hit];//data miss read here
		td->info.emittance[nb_hit][td->current_pix] = draw->light_emittance[nb_hit];
		draw->tex_pos_fp[nb_hit] += draw->step_fp[nb_hit];//data miss read here
	}
}

static inline void	draw_ceil(t_data *data, t_th_draw *td, t_point curr, t_rdir ray, t_draw *draw)
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
	world = reflect_across_mirror(world, draw, curr, &bounce);
	if (world.x < 0 || world.x >= data->map->wid || world.y < 0
		|| world.y >= data->map->len)
		return (setup_color(draw, td, (t_col){0}, 0), (void)0);
	ix = (int)world.x;
	iy = (int)world.y;
	tile = ray.tile_dict[data->map->matrix[iy * data->map->wid + ix]];
	if (!tile)
		return (setup_color(draw, td, (t_col){0}, 0), (void)0);
	tex = tile->tex_ce.img;
	offset = (int)((world.y - iy) * tex->height) * tex->size_line
		+ (int)((world.x - ix) * tex->width);
	light = data->lmap.lmap[(int)(world.x * LMAP_PRECISION)
		+ (int)(world.y * LMAP_PRECISION) * data->lmap.wid].ce_fl;
	int	*base_col = (tex->data + offset);
	color = color_blend(*base_col, light.color, light.emittance);
	setup_color(draw, td, color, bounce - 1);
}

static inline void draw_floor(t_data *data, t_th_draw *td, t_point curr, t_rdir ray, t_draw *draw)
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
	world = reflect_across_mirror(world, draw, curr, &bounce);
	if (world.x < 0 || world.x >= data->map->wid || world.y < 0
		|| world.y >= data->map->len)
		return (setup_color(draw, td, (t_col){0}, 0), (void)0);
	ix = (int)world.x;
	iy = (int)world.y;
	tile = ray.tile_dict[data->map->matrix[iy * data->map->wid + ix]];
	if (!tile)
		return (setup_color(draw, td, (t_col){0}, 0), (void)0);
	tex = tile->tex_fl.img;
	offset = (int)((world.y - iy) * tex->height) * tex->size_line
		+ (int)((world.x - ix) * tex->width);
	light = data->lmap.lmap[(int)(world.x * LMAP_PRECISION)
		+ (int)(world.y * LMAP_PRECISION) * data->lmap.wid].ce_fl;
	int	*base_col = (tex->data + offset);
	color = color_blend(*base_col, light.color, light.emittance);
	setup_color(draw, td, color, bounce - 1);
}

static inline void	draw_wall(t_th_draw *td, t_draw *draw)
{
	t_col	temp;

	temp = color_blend(*(int *)(draw->tex_col[draw->bounces]
			+ (draw->tex_pos_fp[draw->bounces] >> 16) * draw->tex_sizeline[draw->bounces]),
			draw->light_color[draw->bounces], draw->light_emittance[draw->bounces]);
		draw->tex_pos_fp[draw->bounces] += draw->step_fp[draw->bounces];
	setup_color(draw, td, temp, draw->bounces);
}

static inline void	draw_eight(t_data *data, t_th_draw *td, int **img, t_point curr, int *new_line)
{
	int	color[8] __attribute__((aligned(32)));
	int	i;

	get_colors_simd(td->info, color);
	i = -1;
	curr.x -= 8;
	while (++i < 8)
	{
		if (i == *new_line)
		{
			*new_line = -1;
			*img += td->add_next_line + data->img.size_line;
		}
		set_pixels(data, *img, color[i]);
		*img += 2;
	}
	td->current_pix = 0;
}

void	*draw_walls_section(t_th_draw *td)
{
	t_data		*data;
	t_draw		*draw;
	int			*img;
	t_point		curr;
	int			new_line;

	data = get_data();
	img = data->img.data + td->start_x * 2;
	curr.y = -1;
	td->current_pix = 0;
	new_line = -1;
	while (++curr.y < data->render_h)
	{
		curr.x = td->start_x - 1;
		while (++curr.x < td->end_x)
		{
			draw = &data->draw[curr.x];
			if (curr.y >= draw->draw_start[draw->bounces] && curr.y < draw->draw_end[draw->bounces])
				draw_wall(td, draw);
			else if (curr.y < draw->draw_start[draw->bounces])
				draw_ceil(data, td, curr, td->ray_dir, draw);
			else if (curr.y >= draw->draw_end[draw->bounces])
				draw_floor(data, td, curr, td->ray_dir, draw);
			++td->current_pix;
			if (td->current_pix == 8)
				draw_eight(data, td, &img, curr, &new_line);
		}
		new_line = td->current_pix;
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
		data->thread_pool[i].info = (t_simd){0};
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
		tex_end = (line_height / 2 + data->render_h / 2) + 1;
		hit->draw_start[i] = ft_max(tex_start, 0);
		hit->draw_end[i] = ft_min(tex_end, data->render_h);
		line_height = tex_end - tex_start;
		if (line_height == 0)
			line_height = 1;
		hit->step_fp[i] = (hit->texture[i]->height << 16) / line_height;
		hit->tex_pos_fp[i] = (hit->draw_start[i]
			- data->render_h / 2 + line_height / 2) * hit->step_fp[i];
		hit->tex_col[i] = hit->texture[i]->data
			+ (int)(hit->wall[i].pos * hit->texture[i]->width);
	}
}

t_draw	init_draw(t_hit hit)
{
	t_draw	draw;

	draw = (t_draw){0};
	draw.bounces = hit.bounces;
	while (hit.bounces >= 0)
	{
		draw.light_color[hit.bounces] = hit.light[hit.bounces]->color;
		draw.light_emittance[hit.bounces] =  hit.light[hit.bounces]->emittance;
		draw.normal[hit.bounces] = hit.wall[hit.bounces].normal;
		draw.reflectance[hit.bounces] = hit.wall[hit.bounces].reflectance;
		draw.step_fp[hit.bounces] = hit.step_fp[hit.bounces];
		draw.tex_pos_fp[hit.bounces] = hit.tex_pos_fp[hit.bounces];
		draw.tex_col[hit.bounces] = hit.tex_col[hit.bounces];
		draw.tex_sizeline[hit.bounces] = hit.texture[hit.bounces]->size_line;
		draw.draw_start[hit.bounces] = hit.draw_start[hit.bounces];
		draw.draw_end[hit.bounces] = hit.draw_end[hit.bounces];
		draw.hit[hit.bounces] = hit.hit[hit.bounces];
		--hit.bounces;
	}
	return (draw);
}

t_draw cast_ray(t_data *data, t_vec ray_dir)
{
	t_hit	hit;

	hit = (t_hit){0};
	hit = raycast(data, ray_dir, (t_vec){data->map->player.x,
		data->map->player.y});
	init_line_heights(data, &hit, 0, 0);
	return (init_draw(hit));
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
		data->draw[x] = cast_ray(data, ray_dir);
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
