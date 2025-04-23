/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 11:37:46 by hle-hena          #+#    #+#             */
/*   Updated: 2025/04/23 14:23:08 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	calc_err(int *err, int *slope, int da, int db)
{
	if (*err > 0)
	{
		*slope += ft_tern_int(da < 0, -1, 1);
		*err += 2 * (ft_abs(da) - db);
	}
	else
		*err += 2 * ft_abs(da);
}

void	draw_high(t_data *data, t_point start, t_point end, int color)
{
	int		dx;
	int		dy;
	int		err;
	t_point	curr;

	dx = end.x - start.x;
	dy = end.y - start.y;
	err = 2 * ft_abs(dx) - dy;
	curr = (t_point){start.x, start.y};
	while (curr.y <= end.y)
	{
		if (point_is_in_mini_map(data, curr))
			ft_put_pixel(data, curr, color);
		calc_err(&err, &curr.x, dx, dy);
		curr.y += 1;
	}
}

void	draw_low(t_data *data, t_point start, t_point end, int color)
{
	int		dx;
	int		dy;
	int		err;
	t_point	curr;

	dx = end.x - start.x;
	dy = end.y - start.y;
	err = 2 * ft_abs(dy) - dx;
	curr = (t_point){start.x, start.y};
	while (curr.x <= end.x)
	{
		if (point_is_in_mini_map(data, curr))
			ft_put_pixel(data, curr, color);
		calc_err(&err, &curr.y, dy, dx);
		curr.x += 1;
	}
}

void	draw_line(t_data *data, t_point start, t_point end, int color)
{
	if (ft_abs(end.y - start.y) < ft_abs(end.x - start.x))
	{
		if (start.x > end.x)
			return (draw_low(data, end, start, color));
		return (draw_low(data, start, end, color));
	}
	if (start.y > end.y)
		return (draw_high(data, end, start, color));
	return (draw_high(data, start, end, color));
}
