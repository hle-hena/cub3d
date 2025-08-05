/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 22:06:06 by hle-hena          #+#    #+#             */
/*   Updated: 2025/08/05 16:53:02 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

t_col	color_blend(int base_color, int light_color, float emittance)
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

void	set_pixels(t_data *data, int *img, int color)
{
	*(img) = color;
	*(img + 1) = color;
	*(img + data->img[0].size_line) = color;
	*(img + data->img[0].size_line + 1) = color;
}

void	get_colors_simd(t_simd info, int out_colors[8])
{
	t_col_256	prev;
	t_col_256	curr;
	__m256		refl;
	__m256		inv_refl;
	__m256		mask;
	__m256		nb_hit;
	__m256		curr_hit;
	int			i;
	int			max_hit;

	max_hit = 0;
	i = -1;
	while (++i < 8)
	{
		if (info.nb_hit[i] > max_hit)
			max_hit = info.nb_hit[i];
	}
	nb_hit = _mm256_cvtepi32_ps(_mm256_load_si256((__m256i *)info.nb_hit));
	prev.r = MUL_8(LOAD_8(info.fallback.r), info.utils->div);
	prev.g = MUL_8(LOAD_8(info.fallback.g), info.utils->div);
	prev.b = MUL_8(LOAD_8(info.fallback.b), info.utils->div);
	while (--max_hit >= 0)
	{
		curr.r = MUL_8(LOAD_8(info.textures[max_hit].r), info.utils->div);
		curr.g = MUL_8(LOAD_8(info.textures[max_hit].g), info.utils->div);
		curr.b = MUL_8(LOAD_8(info.textures[max_hit].b), info.utils->div);
		curr_hit = SET_8((float)max_hit);
		refl = LOAD_8(info.refl_val[max_hit]);
		inv_refl = SUB_8(info.utils->one, refl);
		mask = _mm256_cmp_ps(curr_hit, nb_hit, _CMP_LT_OQ);
		prev.r = BLEND_8(prev.r, ADD_8(MUL_8(curr.r, MUL_8(refl, prev.r)),
					MUL_8(MUL_8(inv_refl, curr.r),
						MUL_8(LOAD_8(info.light_color[max_hit].r),
							info.utils->div))), mask);
		prev.g = BLEND_8(prev.g, ADD_8(MUL_8(curr.g, MUL_8(refl, prev.g)),
					MUL_8(MUL_8(inv_refl, curr.g),
						MUL_8(LOAD_8(info.light_color[max_hit].g),
							info.utils->div))), mask);
		prev.b = BLEND_8(prev.b, ADD_8(MUL_8(curr.b, MUL_8(refl, prev.b)),
					MUL_8(MUL_8(inv_refl, curr.b),
						MUL_8(LOAD_8(info.light_color[max_hit].b),
							info.utils->div))), mask);
	}
	__m256i r_int = _mm256_cvtps_epi32(MUL_8(prev.r, info.utils->mul));
	__m256i g_int = _mm256_cvtps_epi32(MUL_8(prev.g, info.utils->mul));
	__m256i b_int = _mm256_cvtps_epi32(MUL_8(prev.b, info.utils->mul));
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
void	setup_color(t_draw *draw, t_th_draw *td, t_col fallback, int nb_hit)
{
	int	col;

	td->info.fallback.r[td->current_pix] = fallback.re;
	td->info.fallback.g[td->current_pix] = fallback.gr;
	td->info.fallback.b[td->current_pix] = fallback.bl;
	td->info.nb_hit[td->current_pix] = nb_hit;
	while (--nb_hit >= 0)
	{
		col = *(draw->tex_col[nb_hit]
			+ (draw->tex_pos_fp[nb_hit] >> 16) * draw->tex_sizeline[nb_hit]);
		td->info.textures[nb_hit].r[td->current_pix] = (col >> 16) & 0xFF;
		td->info.textures[nb_hit].g[td->current_pix] = (col >> 8) & 0xFF;
		td->info.textures[nb_hit].b[td->current_pix] = col & 0xFF;
		col = draw->light_color[nb_hit];
		td->info.light_color[nb_hit].r[td->current_pix] = (col >> 16) & 0xFF;
		td->info.light_color[nb_hit].g[td->current_pix] = (col >> 8) & 0xFF;
		td->info.light_color[nb_hit].b[td->current_pix] = col & 0xFF;
		td->info.refl_val[nb_hit][td->current_pix] = draw->reflectance[nb_hit];
		td->info.emittance[nb_hit][td->current_pix] = draw->light_emittance[nb_hit];
		draw->tex_pos_fp[nb_hit] += draw->step_fp[nb_hit];
	}
}

t_vec reflect_across_mirror(t_vec point, t_draw *draw, t_point curr, int *bounce)
{
	t_vec	normal;
	t_vec	hit;
	int		b;
	float	dot;

	b = -1;
	while (++b < draw->bounces)
	{
		if (curr.y < draw->draw_start[b] || curr.y > draw->draw_end[b])
			break;
		normal = draw->normal[b];
		hit = draw->hit[b];
		dot = 2 * ((point.x - hit.x) * normal.x + (point.y - hit.y) * normal.y);
		point.x -= dot * normal.x;
		point.y -= dot * normal.y;
	}
	*bounce = b;
	return (point);
}

void	draw_ceil(t_data *data, t_th_draw *td, t_point curr, t_rdir ray, t_draw *draw)
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
		return (setup_color(draw, td, color_blend(*(int *)(draw->tex_col[draw->bounces]
			+ (draw->tex_pos_fp[draw->bounces] >> 16) * draw->tex_sizeline[draw->bounces]),
			draw->light_color[draw->bounces], draw->light_emittance[draw->bounces]), bounce), (void)0);
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
	setup_color(draw, td, color, bounce);
}

void draw_floor(t_data *data, t_th_draw *td, t_point curr, t_rdir ray, t_draw *draw)
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
		return (setup_color(draw, td, color_blend(*(int *)(draw->tex_col[draw->bounces]
			+ (draw->tex_pos_fp[draw->bounces] >> 16) * draw->tex_sizeline[draw->bounces]),
			draw->light_color[draw->bounces], draw->light_emittance[draw->bounces]), bounce), (void)0);
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
	setup_color(draw, td, color, bounce);
}

void	draw_wall(t_th_draw *td, t_draw *draw)
{
	t_col	temp;

	temp = color_blend(*(int *)(draw->tex_col[draw->bounces]
			+ (draw->tex_pos_fp[draw->bounces] >> 16) * draw->tex_sizeline[draw->bounces]),
			draw->light_color[draw->bounces], draw->light_emittance[draw->bounces]);
		draw->tex_pos_fp[draw->bounces] += draw->step_fp[draw->bounces];
	setup_color(draw, td, temp, draw->bounces);
}

#define BLOCK_X 16
#define BLOCK_Y 16

void	draw_blocked_eight(t_data *data, t_th_draw *td, int **img, int *new_line)
{
	int	color[8] __attribute__((aligned(32)));
	int	i;

	get_colors_simd(td->info, color);
	i = -1;
	while (++i < td->current_pix)
	{
		if (i == *new_line)
		{
			*new_line = -1;
			*img += data->img[0].size_line * 2 - BLOCK_X * 2;
		}
		set_pixels(data, *img, color[i]);
		*img += 2;
	}
	td->current_pix = 0;
}

void	draw_walls_section(t_th_draw *td)
{
	t_data		*data;
	t_draw		*draw;
	int			*img;
	t_point		curr;
	t_point		bcurr;
	int			new_line;

	data = get_data();
	td->current_pix = 0;
	bcurr.y = 0;
	while (bcurr.y < data->render_h)
	{
		bcurr.x = td->start_x;
		while (bcurr.x < td->end_x)
		{
			img = data->img[data->img_buffer].data + bcurr.x * 2 + bcurr.y * data->img[0].size_line * 2;
			curr.y = bcurr.y;
			new_line = -1;
			while (curr.y < bcurr.y + BLOCK_Y && curr.y < data->render_h)
			{
				curr.x = bcurr.x;
				while (curr.x < bcurr.x + BLOCK_X && curr.x < td->end_x)
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
						draw_blocked_eight(data, td, &img, &new_line);
					++curr.x;
				}
				new_line = td->current_pix;
				++curr.y;
			}
			bcurr.x += BLOCK_X;
		}
		bcurr.y += BLOCK_Y;
	}
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
	i = -1;
	while (++i < data->draw_thread)
	{
		pthread_mutex_lock(&data->thread_pool[i].mutex);
		while (!data->thread_pool[i].done)
			pthread_cond_wait(&data->thread_pool[i].cond_done, &data->thread_pool[i].mutex);
		pthread_mutex_unlock(&data->thread_pool[i].mutex);
	}
}

t_draw	init_draw(t_data *data, t_hit hit, int tex_start, int tex_end)
{
	int		i;
	int		line_height;
	t_draw	draw;

	i = -1;
	draw.bounces = hit.bounces;
	while (++i <= hit.bounces)
	{
		if (hit.dist[i] <= 0.0f)
			hit.dist[i] = 0.0001f;
		line_height = (int)(data->render_h * 2 / hit.dist[i]);
		if (line_height == 0)
			line_height = 1;
		tex_start = -line_height / 2 + data->render_h / 2;
		tex_end = (line_height / 2 + data->render_h / 2);
		draw.draw_start[i] = ft_max(tex_start, 0);
		draw.draw_end[i] = ft_min(tex_end, data->render_h);
		line_height = tex_end - tex_start;
		if (line_height == 0)
			line_height = 1;
		draw.step_fp[i] = (hit.texture[i]->height << 16) / line_height;
		draw.tex_pos_fp[i] = (draw.draw_start[i]
			- data->render_h / 2 + line_height / 2) * draw.step_fp[i];
		draw.tex_col[i] = hit.texture[i]->data
			+ (int)(hit.wall[i].pos * hit.texture[i]->width);
		draw.tex_sizeline[i] = hit.texture[i]->size_line;
		draw.light_color[i] = hit.light[i]->color;
		draw.light_emittance[i] = hit.light[i]->emittance;
		draw.reflectance[i] = hit.wall[i].reflectance;
		draw.normal[i] = hit.wall[i].normal;
		draw.hit[i] = hit.hit[i];
	}
	return (draw);
}

t_draw cast_ray(t_data *data, t_vec ray_dir)
{
	t_hit	hit;

	hit = raycast(data, ray_dir, (t_vec){data->map->player.x,
		data->map->player.y});
	return (init_draw(data, hit, 0, 0));
}

void cast_rays(t_data *data)
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
	while (++x < data->render_w)
	{
		cam_x = 2.0f * x / data->render_w - 1.0f;
		ray_dir.x = data->cam.dir.x + data->cam.plane.x * cam_x;
		ray_dir.y = data->cam.dir.y + data->cam.plane.y * cam_x;
		data->draw[x] = cast_ray(data, ray_dir);
	}
	draw_walls(data);
}
