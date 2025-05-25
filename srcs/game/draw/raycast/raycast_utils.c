/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 18:10:46 by hle-hena          #+#    #+#             */
/*   Updated: 2025/05/25 13:52:22 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	init_ray(t_ray *ray, t_vec dir, t_vec origin)
{
	ray->step.x = ft_tern_int(dir.x < 0, -1, 1);
	ray->step.y = ft_tern_int(dir.y < 0, -1, 1);
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
	ray->curr.x = floorf(origin.x);
	ray->curr.y = floorf(origin.y);
	ray->bounce = 0;
	ray->origin = origin;
	ray->dir = dir;
	ray->running = 1;
	ray->precise_dist = 0;
}
