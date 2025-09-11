/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   area.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 14:16:45 by hle-hena          #+#    #+#             */
/*   Updated: 2025/08/28 10:11:10 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

float	area_link(t_link *link)
{
	t_vec	a;
	t_vec	b;
	t_vec	theta;
	float	r;
	float	dtheta;

	a = link->end->coo;
	b = link->start->coo;
	if (link->run_forward)
		a = link->start->coo;
	if (link->run_forward)
		b = link->end->coo;
	if (link->type == 0)
		return (0.5f * (a.x * b.y - b.x * a.y));
	r = sqrtf(vec_len2(vec_sub(a, link->center)));
	theta.x = atan2f(a.y - link->center.y, a.x - link->center.x);
	theta.y = atan2f(b.y - link->center.y, b.x - link->center.x);
	dtheta = theta.y - theta.x;
	if (dtheta <= 0)
		dtheta += 2.0f * PI;
	if (!link->run_forward)
		dtheta = - (2.0f * PI - dtheta);
	return (0.5f * (r * link->center.x * (sinf(theta.y) - sinf(theta.x))
			- r * link->center.y * (cosf(theta.y) - cosf(theta.x))
			+ r * r * dtheta));
}

float	arc_sweep_angle(t_link *arc)
{
	t_vec	ac;
	t_vec	bc;
	float	angle;

	ac = (t_vec){arc->start->coo.x - arc->center.x,
		arc->start->coo.y - arc->center.y};
	bc = (t_vec){arc->end->coo.x - arc->center.x,
		arc->end->coo.y - arc->center.y};
	angle = atan2(bc.y, bc.x) - atan2(ac.y, ac.x);
	if (angle <= -PI)
		angle += 2 * PI;
	if (angle > PI)
		angle -= 2 * PI;
	if (arc->start == arc->end)
		return (2 * PI);
	return (angle);
}

float	perimeter_link(t_link *link)
{
	float	radius;
	float	theta;

	if (link->type == 0)
		return (sqrtf(vec_len2(vec_sub(link->end->coo, link->start->coo))));
	radius = sqrtf(vec_len2(vec_sub(link->start->coo, link->center)));
	theta = arc_sweep_angle(link);
	return (fabsf(radius * theta));
}

float	polygon_area(t_graph *graph, float *perimeter)
{
	int		i;
	float	area;

	*perimeter = 0;
	area = 0;
	i = -1;
	while (++i < graph->nb_links)
	{
		area += area_link(&graph->links[i]);
		*perimeter += perimeter_link(&graph->links[i]);
	}
	return (area);
}
