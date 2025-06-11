/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 22:06:06 by hle-hena          #+#    #+#             */
/*   Updated: 2025/06/11 16:50:39 by hle-hena         ###   ########.fr       */
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

static inline short blend_channel(short base, short other, int refl, int inv)
{
	// int base_refl = (base * refl) >> 8;
	int blend = (base * inv + ((base * other) >> 8) * refl) >> FP_SHIFT;
	if (blend > 255)
		return (255);
	return (blend);
}


static inline t_col blend_color_fast(t_col base_col, t_col refl_col, int refl_fp)
{
	int		inv;

	inv = FP_ONE - refl_fp;
	return (t_col){
		.re = blend_channel(base_col.re, refl_col.re, refl_fp, inv),
		.gr = blend_channel(base_col.gr, refl_col.gr, refl_fp, inv),
		.bl = blend_channel(base_col.bl, refl_col.bl, refl_fp, inv),
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

void get_colors(
	t_hit *hits[8],
	int nb_hit[8],
	t_s_col fallback,
	int isnt_wall[8],
	int out_colors[8]
)
{
	int	i;

	i = -1;
	while (++i < 8)
	{
		out_colors[i] = get_color(hits[i], nb_hit[i],
			(t_col){fallback.r[i], fallback.g[i], fallback.b[i]},
			isnt_wall[i]);
	}
}

// for float :
//    base * (inv_refl + other * refl)

__m256	color_blend_simd(__m256 base_color, __m256 light_color, __m256 emittance)
{
	return (_mm256_mul_ps(_mm256_mul_ps(
		_mm256_mul_ps(base_color, light_color),
		emittance
	), _mm256_set1_ps(1.0f / 65536.0f)));
}

static inline __m256 blend_channel_simd(__m256 base, __m256 other, __m256 refl, __m256 inv_refl)
{
	return (_mm256_mul_ps(
		base,
		_mm256_add_ps(inv_refl, _mm256_mul_ps(_mm256_div_ps(other, _mm256_set1_ps(255.0f)), refl))
	));
}

void get_colors_simd(
	t_hit *hits[8],
	int nb_hit[8],
	t_s_col fallback,
	int out_colors[8]
)
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
	int		i;
	int		max_hit;
	max_hit = 0;
	i = -1;
	while (++i < 8)
	{
		if (nb_hit[i] > max_hit)
			max_hit = nb_hit[i];
	}
	one = _mm256_set1_ps(1);
	max = _mm256_set1_ps(256);
	prev_r = _mm256_load_ps(fallback.r);
	prev_g = _mm256_load_ps(fallback.g);
	prev_b = _mm256_load_ps(fallback.b);
	t_s_col	textures[MAX_BOUNCE] __attribute__((aligned(32))) = {0};
	t_s_col	light_color[MAX_BOUNCE] __attribute__((aligned(32))) = {0};
	float	emittance[MAX_BOUNCE][8] __attribute__((aligned(32))) = {{0}};
	float	refl_val[MAX_BOUNCE][8] __attribute__((aligned(32))) = {{0}};
	for (int hit = 0; hit < max_hit; ++hit)
	{
		for (int i = 0; i < 8; ++i)
		{
			if (hit >= nb_hit[i])
			{
				refl_val[hit][i] = 0;
				emittance[hit][i] = 0;
				textures[hit].r[i] = 0;
				textures[hit].g[i] = 0;
				textures[hit].b[i] = 0;
				light_color[hit].r[i] = 0;
				light_color[hit].g[i] = 0;
				light_color[hit].b[i] = 0;
				continue ;
			}
			int	col = *(int *)((hits[i])->tex_col[hit]
				+ ((hits[i])->tex_pos_fp[hit] >> 16) * (hits[i])->texture[hit]->size_line);
			textures[hit].r[i] = col >> 16 & 0xFF;
			textures[hit].g[i] = col >> 8 & 0xFF;
			textures[hit].b[i] = col & 0xFF;
			col = (hits[i])->light[hit]->color;
			light_color[hit].r[i] = col >> 16 & 0xFF;
			light_color[hit].g[i] = col >> 8 & 0xFF;
			light_color[hit].b[i] = col & 0xFF;
			refl_val[hit][i] = (hits[i])->wall[hit].reflectance;
			emittance[hit][i] = (hits[i])->light[hit]->emittance;
			(hits[i])->tex_pos_fp[hit] += (hits[i])->step_fp[hit];
		}
	}
	while (--max_hit >= 0)
	{
		__m256	em = _mm256_mul_ps(_mm256_load_ps(emittance[max_hit]), max);
		curr_r = color_blend_simd(_mm256_load_ps(textures[max_hit].r),
			_mm256_load_ps(light_color[max_hit].r), em);
		curr_g = color_blend_simd(_mm256_load_ps(textures[max_hit].g),
			_mm256_load_ps(light_color[max_hit].g), em);
		curr_b = color_blend_simd(_mm256_load_ps(textures[max_hit].b),
			_mm256_load_ps(light_color[max_hit].b), em);
		refl = _mm256_load_ps(refl_val[max_hit]);
		inv_refl = _mm256_sub_ps(one, refl);
		prev_r = blend_channel_simd(curr_r, prev_r, refl, inv_refl);
		prev_g = blend_channel_simd(curr_g, prev_g, refl, inv_refl);
		prev_b = blend_channel_simd(curr_b, prev_b, refl, inv_refl);
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

// // SIMD blend function for 4 pixels at once
// // base, refl, inv, refl_fp are __m128i with 4x16-bit channels packed as [re gr bl 0] each?
// // To keep it simple, we will store channels separately in __m128i vectors of 4 shorts per channel.

// static inline __m128i blend_channel_simd(
// 	__m128i base, __m128i refl, __m128i refl_fp, __m128i inv)
// {
// 	// (base * inv + ((base * refl) >> 8) * refl_fp) >> FP_SHIFT

// 	__m128i base_refl = _mm_srli_epi16(_mm_mullo_epi16(base, refl), 8);
// 	__m128i blended = _mm_srli_epi16(
// 		_mm_add_epi16(_mm_mullo_epi16(base, inv),
// 					_mm_mullo_epi16(base_refl, refl_fp)),
// 		FP_SHIFT);
// 	// Clamp to 255
// 	__m128i max_val = _mm_set1_epi16(255);
// 	blended = _mm_min_epi16(blended, max_val);
// 	return blended;
// }

// void get_color_simd(
//     t_hit *hits[4],        // pointers to 4 hit arrays (one per pixel)
//     int nb_hit[4],         // number of hits per pixel
//     t_col fallback[4],     // fallback colors
//     int isnt_wall[4],      // flags per pixel
//     int out_colors[4] // output colors (packed)
// )
// {
//     // We'll keep per-pixel prev_wall_color in arrays of shorts
//     short prev_re[4], prev_gr[4], prev_bl[4];

//     // Initialize prev_wall_color: either fallback or first hit
//     for (int i = 0; i < 4; i++)
//     {
//         if (isnt_wall[i])
//         {
//             prev_re[i] = fallback[i].re;
//             prev_gr[i] = fallback[i].gr;
//             prev_bl[i] = fallback[i].bl;
//         }
//         else
//         {
//             // Load color from texture for hit 0
//             int tex_col_val = *(int *)(hits[i]->tex_col[0]
//                 + (hits[i]->tex_pos_fp[0] >> 16) * hits[i]->texture[0]->size_line);
//             // t_col c = unpack_color(tex_col_val);
//             // Blend with light color and emittance: using your color_blend func (you'd SIMD-ify this too)
//             t_col blended = color_blend(tex_col_val, hits[i]->light[0]->color, hits[i]->light[0]->emittance);
//             prev_re[i] = blended.re;
//             prev_gr[i] = blended.gr;
//             prev_bl[i] = blended.bl;

//             hits[i]->tex_pos_fp[0] += hits[i]->step_fp[0];
//         }
//     }

//     // Convert prev_wall_color arrays to __m128i vectors (16-bit lanes)
//     __m128i prev_re_v = _mm_loadu_si128((__m128i *)prev_re);
//     __m128i prev_gr_v = _mm_loadu_si128((__m128i *)prev_gr);
//     __m128i prev_bl_v = _mm_loadu_si128((__m128i *)prev_bl);

//     // Find max nb_hit to loop from top reflection down
//     int max_nb_hit = 0;
//     for (int i = 0; i < 4; i++)
//         if (nb_hit[i] > max_nb_hit)
//             max_nb_hit = nb_hit[i];

//     // Loop from nb_hit-1 down to 0
//     for (int hit_i = max_nb_hit - 1; hit_i >= 0; hit_i--)
//     {
//         // Create mask for pixels where nb_hit > hit_i (those that need processing)
//         // __m128i mask_nb = _mm_set_epi16(
//         //     (nb_hit[3] > hit_i) ? 0xFFFF : 0,
//         //     (nb_hit[3] > hit_i) ? 0xFFFF : 0,
//         //     (nb_hit[2] > hit_i) ? 0xFFFF : 0,
//         //     (nb_hit[2] > hit_i) ? 0xFFFF : 0,
//         //     (nb_hit[1] > hit_i) ? 0xFFFF : 0,
//         //     (nb_hit[1] > hit_i) ? 0xFFFF : 0,
//         //     (nb_hit[0] > hit_i) ? 0xFFFF : 0,
//         //     (nb_hit[0] > hit_i) ? 0xFFFF : 0
//         // );
//         // Above mask is 8 lanes but we want 4 shorts, so just set for 4 lanes:
//         // Let's do it simpler:
//         // We'll use 4 lanes 16-bit: [pixel3 pixel2 pixel1 pixel0]
//         // So:
//         __m128i mask;
//         mask = _mm_set_epi16(
//             0,
//             0,
//             (nb_hit[3] > hit_i) ? 0xFFFF : 0,
//             0,
//             0,
//             (nb_hit[2] > hit_i) ? 0xFFFF : 0,
//             0,
//             (nb_hit[1] > hit_i) ? 0xFFFF : 0
//         );
//         // This is complicated and incorrect for 4-lane use, so better do 32-bit lane mask:
//         // Let's do mask as 32-bit lane (__m128i), using _mm_set_epi32

//         // __m128i mask32 = _mm_set_epi32(
//         //     (nb_hit[3] > hit_i) ? -1 : 0,
//         //     (nb_hit[2] > hit_i) ? -1 : 0,
//         //     (nb_hit[1] > hit_i) ? -1 : 0,
//         //     (nb_hit[0] > hit_i) ? -1 : 0
//         // );

//         // Load wall_color channels for the current hit for each pixel, or fallback if not present
//         short wall_re[4], wall_gr[4], wall_bl[4];

//         for (int i = 0; i < 4; i++)
//         {
//             if (nb_hit[i] > hit_i)
//             {
//                 int tex_col_val = *(int *)(hits[i]->tex_col[hit_i]
//                     + (hits[i]->tex_pos_fp[hit_i] >> 16) * hits[i]->texture[hit_i]->size_line);
//                 t_col blended = color_blend(tex_col_val, hits[i]->light[hit_i]->color, hits[i]->light[hit_i]->emittance);

//                 wall_re[i] = blended.re;
//                 wall_gr[i] = blended.gr;
//                 wall_bl[i] = blended.bl;
//             }
//             else
//             {
//                 // Use zero if no hit to blend (or fallback color?)
//                 wall_re[i] = 0;
//                 wall_gr[i] = 0;
//                 wall_bl[i] = 0;
//             }
//             hits[i]->tex_pos_fp[hit_i] += hits[i]->step_fp[hit_i];
//         }

//         __m128i wall_re_v = _mm_loadu_si128((__m128i *)wall_re);
//         __m128i wall_gr_v = _mm_loadu_si128((__m128i *)wall_gr);
//         __m128i wall_bl_v = _mm_loadu_si128((__m128i *)wall_bl);

//         // Get reflectance vector as 16-bit values (assuming reflectance stored as int, convert to FP)
//         short refl_fp_arr[4];
//         for (int i = 0; i < 4; i++)
//             refl_fp_arr[i] = TO_FP(hits[i]->wall[hit_i].reflectance);
//         __m128i refl_fp_v = _mm_loadu_si128((__m128i *)refl_fp_arr);

//         __m128i inv_v = _mm_sub_epi16(_mm_set1_epi16((short)FP_ONE), refl_fp_v);

//         // Blend each channel SIMD
//         prev_re_v = blend_channel_simd(prev_re_v, wall_re_v, refl_fp_v, inv_v);
//         prev_gr_v = blend_channel_simd(prev_gr_v, wall_gr_v, refl_fp_v, inv_v);
//         prev_bl_v = blend_channel_simd(prev_bl_v, wall_bl_v, refl_fp_v, inv_v);
//     }

//     // Store back blended colors
//     _mm_storeu_si128((__m128i *)prev_re, prev_re_v);
//     _mm_storeu_si128((__m128i *)prev_gr, prev_gr_v);
//     _mm_storeu_si128((__m128i *)prev_bl, prev_bl_v);

//     // Pack final colors to uint32_t (0x00RRGGBB)
//     for (int i = 0; i < 4; i++)
//     {
//         out_colors[i] = (prev_re[i] << 16) | (prev_gr[i] << 8) | prev_bl[i];
//     }
// }

static inline void draw_ceil(t_data *data, t_th_draw *td, t_point curr, t_rdir ray, /* char *img, */ t_hit *hit)
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
	{
		td->info.hits[td->current_pix] = hit;
		td->info.fallback.r[td->current_pix] = 0.0f;
		td->info.fallback.g[td->current_pix] = 0.0f;
		td->info.fallback.b[td->current_pix] = 0.0f;
		td->info.isnt_wall[td->current_pix] = 1;
		td->info.nb_hit[td->current_pix] = 0;
		return ;
	}
	ix = (int)world.x;
	iy = (int)world.y;
	tile = ray.tile_dict[data->map->matrix[iy * data->map->wid + ix]];
	if (!tile)
	{
		td->info.hits[td->current_pix] = hit;
		td->info.fallback.r[td->current_pix] = 0.0f;
		td->info.fallback.g[td->current_pix] = 0.0f;
		td->info.fallback.b[td->current_pix] = 0.0f;
		td->info.isnt_wall[td->current_pix] = 1;
		td->info.nb_hit[td->current_pix] = 0;
		return ;
	}
	tex = tile->tex_ce.img;
	offset = (int)((world.y - iy) * tex->height) * tex->size_line
		+ (int)((world.x - ix) * tex->width) * tex->bpp;
	light = data->lmap.lmap[(int)(world.x * LMAP_PRECISION)
		+ (int)(world.y * LMAP_PRECISION) * data->lmap.wid].ce_fl;
	int	*base_col = (int *)(tex->data + offset);
	color = color_blend(*base_col, light.color, light.emittance);
	td->info.hits[td->current_pix] = hit;
	td->info.fallback.r[td->current_pix] = (float)color.re;
	td->info.fallback.g[td->current_pix] = (float)color.gr;
	td->info.fallback.b[td->current_pix] = (float)color.bl;
	td->info.isnt_wall[td->current_pix] = 1;
	td->info.nb_hit[td->current_pix] = bounce - 1;
}

static inline void draw_floor(t_data *data, t_th_draw *td, t_point curr, t_rdir ray, /* char *img, */ t_hit *hit)
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
	{
		td->info.hits[td->current_pix] = hit;
		td->info.fallback.r[td->current_pix] = 0.0f;
		td->info.fallback.g[td->current_pix] = 0.0f;
		td->info.fallback.b[td->current_pix] = 0.0f;
		td->info.isnt_wall[td->current_pix] = 1;
		td->info.nb_hit[td->current_pix] = 0;
		return ;
	}
	ix = (int)world.x;
	iy = (int)world.y;
	tile = ray.tile_dict[data->map->matrix[iy * data->map->wid + ix]];
	if (!tile)
	{
		td->info.hits[td->current_pix] = hit;
		td->info.fallback.r[td->current_pix] = 0.0f;
		td->info.fallback.g[td->current_pix] = 0.0f;
		td->info.fallback.b[td->current_pix] = 0.0f;
		td->info.isnt_wall[td->current_pix] = 1;
		td->info.nb_hit[td->current_pix] = 0;
		return ;
	}
	tex = tile->tex_fl.img;
	offset = (int)((world.y - iy) * tex->height) * tex->size_line
		+ (int)((world.x - ix) * tex->width) * tex->bpp;
	light = data->lmap.lmap[(int)(world.x * LMAP_PRECISION)
		+ (int)(world.y * LMAP_PRECISION) * data->lmap.wid].ce_fl;
	int	*base_col = (int *)(tex->data + offset);
	color = color_blend(*base_col, light.color, light.emittance);
	td->info.hits[td->current_pix] = hit;
	td->info.fallback.r[td->current_pix] = (float)color.re;
	td->info.fallback.g[td->current_pix] = (float)color.gr;
	td->info.fallback.b[td->current_pix] = (float)color.bl;
	td->info.isnt_wall[td->current_pix] = 1;
	td->info.nb_hit[td->current_pix] = bounce - 1;
}

static inline void	draw_wall(t_data *data, t_th_draw *td/* , char *img */, t_hit *hit)
{

	td->info.hits[td->current_pix] = hit;
	if (data->simd)
	{
		t_col	temp;
		temp = color_blend(*(int *)(hit->tex_col[hit->bounces]
				+ (hit->tex_pos_fp[hit->bounces] >> 16) * hit->texture[hit->bounces]->size_line),
				hit->light[hit->bounces]->color, hit->light[hit->bounces]->emittance);
			hit->tex_pos_fp[hit->bounces] += hit->step_fp[hit->bounces];
		td->info.fallback.r[td->current_pix] = (float)temp.re;
		td->info.fallback.g[td->current_pix] = (float)temp.gr;
		td->info.fallback.b[td->current_pix] = (float)temp.bl;
	}
	else
	{
		td->info.fallback.r[td->current_pix] = 0;
		td->info.fallback.g[td->current_pix] = 0;
		td->info.fallback.b[td->current_pix] = 0;
	}
	td->info.isnt_wall[td->current_pix] = 0;
	td->info.nb_hit[td->current_pix] = hit->bounces;
}

static inline void	draw_eight(t_data *data, t_th_draw *td, char **img, t_point curr, int *new_line)
{
	int	color[8] __attribute__((aligned(32)));
	int	i;

	if (data->simd)
		get_colors_simd(td->info.hits, td->info.nb_hit, td->info.fallback, color);
	else
		get_colors(td->info.hits, td->info.nb_hit, td->info.fallback, td->info.isnt_wall, color);
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
		*img += data->img.bpp * 2;
	}
	td->current_pix = 0;
}

void	*draw_walls_section(t_th_draw *td)
{
	t_data		*data;
	t_hit		*hit;
	char		*img;
	t_point		curr;
	int			new_line;

	data = get_data();
	img = data->img.data + td->start_x * 2 * data->img.bpp;
	curr.y = -1;
	td->current_pix = 0;
	new_line = -1;
	while (++curr.y < data->render_h)
	{
		curr.x = td->start_x - 1;
		while (++curr.x < td->end_x)
		{
			hit = &data->hits[curr.x];
			if (curr.y >= hit->draw_start[hit->bounces] && curr.y < hit->draw_end[hit->bounces])
				draw_wall(data, td, hit);
			else if (curr.y < hit->draw_start[hit->bounces])
				draw_ceil(data, td, curr, td->ray_dir, hit);
			else if (curr.y >= hit->draw_end[hit->bounces])
				draw_floor(data, td, curr, td->ray_dir, hit);
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
