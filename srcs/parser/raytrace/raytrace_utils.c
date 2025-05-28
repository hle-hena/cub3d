/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raytrace_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 10:13:01 by hle-hena          #+#    #+#             */
/*   Updated: 2025/05/28 17:47:43 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	is_correct_flight(void *content, void *to_find)
{
	t_vec	cont_normal;
	t_vec	find_normal;

	cont_normal = ((t_flight *)content)->normal;
	find_normal = *(t_vec *)to_find;
	return (fabs(cont_normal.x - find_normal.x) < 0.1
		&& fabs(cont_normal.y - find_normal.y) < 0.1);
}

void	init_trace(t_trace *ray, t_vec dir, t_vec origin, float emittance)
{
	origin = (t_vec){origin.x * LMAP_PRECISION, origin.y * LMAP_PRECISION};
	ray->step.x = 1;
	if (dir.x < 0)
		ray->step.x = -1;
	ray->step.y = 1;
	if (dir.y < 0)
		ray->step.y = -1;
	ray->slope.x = HUGE_VALF;
	if (dir.x != 0)
		ray->slope.x = fabs(1.0f / dir.x);
	ray->slope.y = HUGE_VALF;
	if (dir.y != 0)
		ray->slope.y = fabs(1.0f / dir.y);
	if (dir.x < 0)
		ray->dist.x = (origin.x - floorf(origin.x)) * ray->slope.x;
	else
		ray->dist.x = (ceilf(origin.x) - origin.x) * ray->slope.x;
	if (dir.y < 0)
		ray->dist.y = (origin.y - floorf(origin.y)) * ray->slope.y;
	else
		ray->dist.y = (ceilf(origin.y) - origin.y) * ray->slope.y;
	ray->curr = (t_point){floorf(origin.x), floorf(origin.y)};
	ray->bounce = 0;
	ray->origin = origin;
	ray->dir = dir;
	ray->running = 1;
	ray->precise_dist = 0;
	ray->last_dist = 0;
	ray->emittance = emittance;
}
