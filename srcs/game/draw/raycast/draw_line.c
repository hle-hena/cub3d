/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 19:54:48 by hle-hena          #+#    #+#             */
/*   Updated: 2025/04/13 20:06:49 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	draw_line_in_direction(t_data *data, t_point start, t_vec dir,
	float dist)
{
	float	step = 0.5;
	float	curr_dist = 0.0;
	t_point	curr;

	while (curr_dist < dist)
	{
		curr.x = start.x + dir.x * curr_dist;
		curr.y = start.y + dir.y * curr_dist;
		if (point_is_in_mini_map(data, curr))
			ft_put_pixel(data, curr, 0x0000FF);
		curr_dist += step;
	}
}
