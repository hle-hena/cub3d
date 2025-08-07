/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_color.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 16:40:10 by hle-hena          #+#    #+#             */
/*   Updated: 2025/08/07 16:40:17 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static inline float	to_linear(float c)
{
	return ((c / 255.0f) * (c / 255.0f));
}

static inline float	to_srgb(float c)
{
	return (sqrtf(c) * 255.0f);
}

int	add_col_val_physical(int col1, int col2, float weight1,
	float weight2)
{
	t_col	final;

	final.re = (int)to_srgb((weight1 * to_linear((col1 >> 16) & 0xFF))
			+ (to_linear((col2 >> 16) & 0xFF) * weight2));
	final.gr = (int)to_srgb((weight1 * to_linear((col1 >> 8) & 0xFF))
			+ (to_linear((col2 >> 8) & 0xFF) * weight2));
	final.bl = (int)to_srgb((weight1 * to_linear(col1 & 0xFF))
			+ (to_linear(col2 & 0xFF) * weight2));
	if (final.re > 255)
		final.re = 255;
	if (final.gr > 255)
		final.gr = 255;
	if (final.bl > 255)
		final.bl = 255;
	return ((final.re << 16) | (final.gr << 8) | final.bl);
}
