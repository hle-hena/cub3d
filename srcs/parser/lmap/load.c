/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 12:08:29 by hle-hena          #+#    #+#             */
/*   Updated: 2025/04/29 15:29:28 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static inline void	add_emittance(t_tlight *final, t_tlight *temp, int i)
{
	(final + i)->ce_fl += (temp + i)->ce_fl;
	if ((final + i)->ce_fl > 1)
		(final + i)->ce_fl = 1;
	(final + i)->no_so += (temp + i)->no_so;
	if ((final + i)->no_so > 1)
		(final + i)->no_so = 1;
	(final + i)->we_ea += (temp + i)->we_ea;
	if ((final + i)->we_ea > 1)
		(final + i)->we_ea = 1;
}

static inline float	to_linear(float c)
{
	return ((c / 255.0f) * (c / 255.0f));
}

static inline float	to_srgb(float c)
{
	return (sqrtf(c) * 255.0f);
}

static inline int	add_col_val_physical(int col1, int col2, float weight)
{
	float r1 = to_linear((col1 >> 16) & 0xFF);
	float g1 = to_linear((col1 >> 8) & 0xFF);
	float b1 = to_linear(col1 & 0xFF);
	float r2 = to_linear((col2 >> 16) & 0xFF) * weight;
	float g2 = to_linear((col2 >> 8) & 0xFF) * weight;
	float b2 = to_linear(col2 & 0xFF) * weight;
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

static inline void add_color(t_tlight *final, t_tlight *temp, int i)
{
	float weight_ce = (temp + i)->ce_fl;
	float weight_we = (temp + i)->we_ea;
	float weight_no = (temp + i)->no_so;

	(final + i)->col_ce = add_col_val_physical((final + i)->col_ce, (temp + i)->col_ce, weight_ce);
	(final + i)->col_we = add_col_val_physical((final + i)->col_we, (temp + i)->col_we, weight_we);
	(final + i)->col_no = add_col_val_physical((final + i)->col_no, (temp + i)->col_no, weight_no);
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
			add_emittance(final, temp, i);
			add_color(final, temp, i);
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
		deg += 0.005;
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
