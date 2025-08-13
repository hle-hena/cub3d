/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersect_sseg.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 11:31:12 by hle-hena          #+#    #+#             */
/*   Updated: 2025/08/13 11:32:27 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

t_inter	intersect_sseg(t_vec origin, t_vec dir, t_wpath seg)
{
	t_vec	seg_dir;
	t_vec	delta;
	float	det;
	float	t;
	float	u;

	seg_dir = (t_vec){seg.end.x - seg.start.x, seg.end.y - seg.start.y};
	det = -dir.x * seg_dir.y + dir.y * seg_dir.x;
	if (fabs(det) < FLT_EPSILON)
		return ((t_inter){(t_vec){0}, (t_vec){0}, -1, -1});
	delta = (t_vec){seg.start.x - origin.x, seg.start.y - origin.y};
	t = (delta.x * -seg_dir.y + delta.y * seg_dir.x) / det;
	u = (-dir.y * delta.x + dir.x * delta.y) / det;
	seg_dir = vec_normalize((t_vec){-seg_dir.y, seg_dir.x});
	if (vec_dot(dir, seg_dir) > 0)
		return ((t_inter){(t_vec){0}, (t_vec){0}, -1, -1});
	if (t >= 0 && u >= 0 && u <= 1)
		return ((t_inter){(t_vec){0}, seg_dir, u, t});
	return ((t_inter){(t_vec){0}, (t_vec){0}, -1, -1});
}
