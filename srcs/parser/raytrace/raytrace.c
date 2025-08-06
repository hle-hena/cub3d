/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raytrace.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 10:11:09 by hle-hena          #+#    #+#             */
/*   Updated: 2025/08/06 16:05:07 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	raytrace(t_data *data, t_light light, t_vec dir)
{
	t_trace	ray;

	init_trace(&ray, dir, light.pos, light.emittance);
	light_floor(data, &ray, light);
	handle_light(data, &ray, light);
	while (ray.running)
	{
		if (ray.dist.x < ray.dist.y)
		{
			ray.precise_dist = ray.dist.x;
			ray.curr.x += ray.step.x;
			ray.dist.x += ray.slope.x;
			ray.side = 0;
		}
		else
		{
			ray.precise_dist = ray.dist.y;
			ray.curr.y += ray.step.y;
			ray.dist.y += ray.slope.y;
			ray.side = 1;
		}
		handle_light(data, &ray, light);
	}
}
