/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_circle.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 23:18:32 by hle-hena          #+#    #+#             */
/*   Updated: 2025/04/13 15:28:28 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	draw_disk(t_data *data, t_point center, int rad, int color)
{
	t_point	current;

	current.y = center.y - rad;
	while (current.y != center.y + rad
		&& current.y >= 0 && current.y < data->win_len)
	{
		current.x = center.x - rad;
		while (current.x != center.x + rad
			&& current.x >= 0 && current.x < data->win_wid)
		{
			if (sqrt(pow(current.x - center.x, 2)
					+ pow(current.y - center.y, 2)) <= rad)
				ft_put_pixel(data, current, color);
			current.x++;
		}
		current.y++;
	}
}

void	draw_circle(t_data *data, t_point center, int radius, int color)
{
	int	x;
	int	y;
	int	d;

	x = -1;
	y = radius;
	d = 3 - 2 * radius;
	while (++x <= y)
	{
		ft_put_pixel(data, (t_point){center.x + x, center.y + y, 0}, color);
		ft_put_pixel(data, (t_point){center.x - x, center.y + y, 0}, color);
		ft_put_pixel(data, (t_point){center.x + x, center.y - y, 0}, color);
		ft_put_pixel(data, (t_point){center.x - x, center.y - y, 0}, color);
		ft_put_pixel(data, (t_point){center.x + y, center.y + x, 0}, color);
		ft_put_pixel(data, (t_point){center.x - y, center.y + x, 0}, color);
		ft_put_pixel(data, (t_point){center.x + y, center.y - x, 0}, color);
		ft_put_pixel(data, (t_point){center.x - y, center.y - x, 0}, color);
		if (d < 0)
			d += 4 * x + 6;
		else
		{
			d += 4 * (x - y) + 10;
			y--;
		}
	}
}
