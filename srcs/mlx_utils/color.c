/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 16:25:58 by hle-hena          #+#    #+#             */
/*   Updated: 2025/05/19 16:57:29 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	calc_color(t_col col)
{
	return ((col.re << 16) | (col.gr << 8) | col.bl);
}

t_col	rev_calc_color(int col)
{
	return ((t_col){col >> 16 & 0xFF, col >> 8 & 0xFF, col & 0xFF});
}

int	interpolate_color(int col1, int col2, float percent)
{
	t_col	c1 = rev_calc_color(col1);
	t_col	c2 = rev_calc_color(col2);
	t_col	result;

	result.re = (int)(c1.re + (c2.re - c1.re) * percent);
	result.gr = (int)(c1.gr + (c2.gr - c1.gr) * percent);
	result.bl = (int)(c1.bl + (c2.bl - c1.bl) * percent);
	return (calc_color(result));
}
