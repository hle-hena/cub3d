/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 12:08:29 by hle-hena          #+#    #+#             */
/*   Updated: 2025/05/22 15:44:15 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static inline float	to_linear(float c)
{
	return ((c / 255.0f) * (c / 255.0f));
}

static inline float	to_srgb(float c)
{
	return (sqrtf(c) * 255.0f);
}

static inline int	add_col_val_physical(int col1, int col2, float weight1,
	float weight2)
{
	float r1 = to_linear((col1 >> 16) & 0xFF) * weight1;
	float g1 = to_linear((col1 >> 8) & 0xFF) * weight1;
	float b1 = to_linear(col1 & 0xFF) * weight1;
	float r2 = to_linear((col2 >> 16) & 0xFF) * weight2;
	float g2 = to_linear((col2 >> 8) & 0xFF) * weight2;
	float b2 = to_linear(col2 & 0xFF) * weight2;
	float r = r1 + r2;
	float g = g1 + g2;
	float b = b1 + b2;
	int ri = (int)to_srgb(r);
	int gi = (int)to_srgb(g);
	int bi = (int)to_srgb(b);
	if (ri > 255) ri = 255;
	if (gi > 255) gi = 255;
	if (bi > 255) bi = 255;
	return (ri << 16) | (gi << 8) | bi;
}

static inline void add_color(t_tlight *final, t_tlight *temp)
{
	t_list		*found;
	t_flight	*final_flight;
	t_flight	*temp_flight;

	while (temp->flight)
	{
		temp_flight = (t_flight *)temp->flight->content;
		found = ft_lstchr(final->flight,
			&temp_flight->normal, is_correct_flight);
		if (found)
		{
			final_flight = (t_flight *)found->content;
			final_flight->color = add_col_val_physical(final_flight->color,
				temp_flight->color, final_flight->emittance, temp_flight->emittance);
			final_flight->emittance += temp_flight->emittance;
			if (final_flight->emittance > 1)
				final_flight->emittance = 1;
		}
		else
			add_link(&final->flight, temp_flight);
		temp->flight = temp->flight->next;
	}
	final->ce_fl.color = add_col_val_physical(final->ce_fl.color,
		temp->ce_fl.color, final->ce_fl.emittance, temp->ce_fl.emittance);
	final->ce_fl.emittance += temp->ce_fl.emittance;
	if (final->ce_fl.emittance > 1)
		final->ce_fl.emittance = 1;
}

void	add_lmap(t_data *data, t_tlight *final, t_tlight *temp)
{
	int		x;
	int		y;
	int		i;

	y = -1;
	i = 0;
	while (++y < data->lmap.len)
	{
		x = -1;
		while (++x < data->lmap.wid)
		{
			add_color(final + i, temp + i);
			i++;
		}
	}
}

void	raytrace_source(t_data *data, t_light light)
{
	float	deg;
	t_vec	dir;

	deg = 0;
	printf("Doing (%.2f, %.2f) for %.2f\n", light.pos.x, light.pos.y, light.emittance);
	light.pos.x += 0.001;
	light.pos.y += 0.001;
	while (deg < 360)
	{
		dir.x = cos(deg * PI / 180);
		dir.y = sin(deg * PI / 180);
		raytrace(data, light, dir);
		deg += 0.001;
	}
}

int	create_lmap(t_data *data)
{
	int			i;
	t_lmap		*lmap;
	t_tlight	*temp;

	lmap = &data->lmap;
	lmap->wid = data->map->wid * LMAP_PRECISION;
	lmap->len = data->map->len * LMAP_PRECISION;
	data->lmap.lmap = ft_calloc(lmap->wid * lmap->len, sizeof(t_tlight));
	temp = ft_calloc(lmap->wid * lmap->len, sizeof(t_tlight));
	if (!lmap->lmap || !temp)
		return (1);
	if (!lmap->lights)
		return (0);
	i = -1;
	while (++i < lmap->nb_ls)
	{
		raytrace_source(data, lmap->lights[i]);
		add_lmap(data, temp, data->lmap.lmap);
		ft_bzero(data->lmap.lmap, lmap->wid * lmap->len * sizeof(t_tlight));
	}
	ft_del((void **)&data->lmap.lmap);
	data->lmap.lmap = temp;
	return (0);
}
