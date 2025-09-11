/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wall_intersect.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 13:37:04 by hle-hena          #+#    #+#             */
/*   Updated: 2025/09/11 14:51:21 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D_bonus.h"

static inline t_wpath	line(t_point curr, t_wpath base)
{
	return ((t_wpath){(t_vec){base.start.x + curr.x, base.start.y + curr.y},
		(t_vec){base.end.x + curr.x, base.end.y + curr.y}, (t_vec){base.center.x
			+ curr.x, base.center.y + curr.y}, (t_text){0}, (t_vec){0}, 0, 0, 0,
			1, base.mode, base.run_forward});
}

int	does_hit(t_list	*wpath, t_ray *ray, t_wpath *wall)
{
	t_inter	inter;
	float	closest_dist;

	closest_dist = -1;
	while (wpath)
	{
		inter = intersect_switch(ray->origin, ray->dir,
				line(ray->curr, *(t_wpath *)wpath->content));
		if (inter.pos > 0 - FLT_EPSILON
			&& (inter.dist < closest_dist || closest_dist == -1))
		{
			*wall = *(t_wpath *)wpath->content;
			closest_dist = inter.dist;
			wall->pos = inter.pos * (wall->pos_end - wall->pos_start)
				+ wall->pos_start;
			wall->normal = inter.normal;
		}
		wpath = wpath->next;
	}
	if (closest_dist < 0)
		return (0);
	ray->precise_dist = closest_dist;
	return (1);
}
