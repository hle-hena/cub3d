/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_colors.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 11:42:56 by hle-hena          #+#    #+#             */
/*   Updated: 2025/08/28 10:54:39 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void	mix_mirror_colors(t_col_256 *prev, int max_hit, t_simd info,
	__m256 nb_hit)
{
	t_col_256	curr;
	__m256		refl;
	__m256		inv_refl;
	__m256		mask;

	while (--max_hit >= 0)
	{
		curr.r = MUL_8(LOAD_8(info.textures[max_hit].r), info.utils->div);
		curr.g = MUL_8(LOAD_8(info.textures[max_hit].g), info.utils->div);
		curr.b = MUL_8(LOAD_8(info.textures[max_hit].b), info.utils->div);
		refl = LOAD_8(info.refl_val[max_hit]);
		inv_refl = SUB_8(info.utils->one, refl);
		mask = _mm256_cmp_ps(SET_8((float)max_hit), nb_hit, _CMP_LT_OQ);
		prev->r = BLEND_8(prev->r, ADD_8(MUL_8(curr.r, MUL_8(refl, prev->r)),
					MUL_8(MUL_8(inv_refl, curr.r), MUL_8(LOAD_8(info.light_color
							[max_hit].r), info.utils->div))), mask);
		prev->g = BLEND_8(prev->g, ADD_8(MUL_8(curr.g, MUL_8(refl, prev->g)),
					MUL_8(MUL_8(inv_refl, curr.g), MUL_8(LOAD_8(info.light_color
							[max_hit].g), info.utils->div))), mask);
		prev->b = BLEND_8(prev->b, ADD_8(MUL_8(curr.b, MUL_8(refl, prev->b)),
					MUL_8(MUL_8(inv_refl, curr.b), MUL_8(LOAD_8(info.light_color
							[max_hit].b), info.utils->div))), mask);
	}
}

void	get_colors_simd(t_simd info, int out_colors[8])
{
	t_col_256	prev;
	t_col_256i	final;
	__m256		nb_hit;
	int			i;
	int			max_hit;

	max_hit = 0;
	i = -1;
	while (++i < 8)
		if (info.nb_hit[i] > max_hit)
			max_hit = info.nb_hit[i];
	nb_hit = _mm256_cvtepi32_ps(_mm256_load_si256((__m256i *)info.nb_hit));
	prev.r = MUL_8(LOAD_8(info.fallback.r), info.utils->div);
	prev.g = MUL_8(LOAD_8(info.fallback.g), info.utils->div);
	prev.b = MUL_8(LOAD_8(info.fallback.b), info.utils->div);
	mix_mirror_colors(&prev, max_hit, info, nb_hit);
	final.r = _mm256_cvtps_epi32(MUL_8(prev.r, info.utils->mul));
	final.g = _mm256_cvtps_epi32(MUL_8(prev.g, info.utils->mul));
	final.b = _mm256_cvtps_epi32(MUL_8(prev.b, info.utils->mul));
	_mm256_store_si256((__m256i *)out_colors,
		_mm256_or_si256(
			_mm256_or_si256(
				_mm256_slli_epi32(final.r, 16),
				_mm256_slli_epi32(final.g, 8)
				), final.b));
}

t_col	color_blend(int base_color, int light_color, float emittance)
{
	int		em;
	t_col	base;
	t_col	light;
	t_col	final;

	emittance = (LIGHT_ON * 1) + (!LIGHT_ON * emittance);
	light_color = (LIGHT_ON * 0xFFFFFF) + (!LIGHT_ON * light_color);
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
