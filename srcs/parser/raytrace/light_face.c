/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light_face.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 16:00:24 by hle-hena          #+#    #+#             */
/*   Updated: 2025/09/02 15:45:34 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void	light_flight(t_flight *flight, float emittance,
	t_light light)
{
	if (flight->ibuffer != light.index)
	{
		if (flight->ibuffer == 0)
		{
			flight->color = calc_color((t_col){light.color.re * emittance,
					light.color.gr * emittance, light.color.bl * emittance});
			flight->emittance = emittance;
		}
		else
		{
			flight->color = add_col_val_physical(flight->color, calc_color(
						(t_col){light.color.re * emittance, light.color.gr
						* emittance, light.color.bl * emittance}),
					flight->emittance, emittance);
			flight->emittance = fmin(flight->emittance + emittance, 1);
		}
		flight->ibuffer = light.index;
	}
}

void	light_adjacent(t_data *data, t_trace *ray, t_wpath wall, t_light light)
{
	t_vec	offset;
	float	emittance;
	t_point	direct;
	t_point	diag;

	emittance = ray->angle_factor * ray->emittance;
	offset.x = ray->origin.x - (int)ray->origin.x;
	offset.y = ray->origin.y - (int)ray->origin.y;
	diag.x = ray->curr.x + (offset.x < 0.5f) * -1
		+ (offset.x > 1.0f - 0.5f) * 1;
	diag.y = ray->curr.y + (offset.y < 0.5f) * -1
		+ (offset.y > 1.0f - 0.5f) * 1;
	diag.x = ft_min(ft_max(diag.x, 0), data->lmap.wid - 1);
	diag.y = ft_min(ft_max(diag.y, 0), data->lmap.len - 1);
	direct.x = diag.x;
	direct.y = ray->curr.y;
	light_flight(find_flight(data, direct, wall), emittance, light);
	direct.x = ray->curr.x;
	direct.y = diag.y;
	light_flight(find_flight(data, direct, wall), emittance, light);
	light_flight(find_flight(data, diag, wall), emittance, light);
}

void	light_wall(t_data *data, t_trace *ray, t_wpath wall,
	t_light light)
{
	float		emittance;

	emittance = ray->angle_factor * ray->emittance;
	ray->curr = (t_point){(int)ray->origin.x, (int)ray->origin.y};
	light_flight(find_flight(data, ray->curr, wall), emittance, light);
	light_adjacent(data, ray, wall, light);
	ray->running = 0;
	ray->emittance -= (1 - wall.reflectance);
}

void	light_floor(t_data *data, t_trace *ray, t_light light)
{
	t_tlight	*tlight;
	float		angle_factor;

	tlight = (data->lmap.lmap + ray->curr.x + ray->curr.y * data->lmap.wid);
	ray->emittance = (ray->base_emittance)
		/ (1 + ATT_COEF * pow(ray->precise_dist / LMAP_PRECISION, 2));
	angle_factor = 0.05f + (1.0f - 0.05f) / (1.0f + powf((ray->precise_dist
					/ LMAP_PRECISION) / 0.8f, 1.57f));
	angle_factor = sqrtf(sqrtf(angle_factor));
	light_flight(&tlight->ce_fl, ray->emittance * angle_factor, light);
}
