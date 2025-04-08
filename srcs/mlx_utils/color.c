/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 16:25:58 by hle-hena          #+#    #+#             */
/*   Updated: 2025/04/04 16:26:33 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	calc_color(t_col col)
{
	return ((col.r << 16) | (col.g << 8) | col.b);
}

t_col	rev_calc_color(int col)
{
	return ((t_col){col >> 16 & 0xFF, col >> 8 & 0xFF, col & 0xFF});
}