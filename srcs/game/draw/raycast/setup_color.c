/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_color.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 11:44:56 by hle-hena          #+#    #+#             */
/*   Updated: 2025/08/28 10:54:31 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

//This function is cache-miss-read heavy ...
void	setup_color(t_draw *draw, t_th_draw *td, t_col fallback, int nb_hit)
{
	int	col;

	td->info.fallback.r[td->current_pix] = fallback.re;
	td->info.fallback.g[td->current_pix] = fallback.gr;
	td->info.fallback.b[td->current_pix] = fallback.bl;
	td->info.nb_hit[td->current_pix] = nb_hit;
	while (--nb_hit >= 0)
	{
		col = *(draw->tex_col[nb_hit] + (draw->tex_pos_fp[nb_hit] >> 16)
				* draw->tex_sizeline[nb_hit]);
		td->info.textures[nb_hit].r[td->current_pix] = (col >> 16) & 0xFF;
		td->info.textures[nb_hit].g[td->current_pix] = (col >> 8) & 0xFF;
		td->info.textures[nb_hit].b[td->current_pix] = col & 0xFF;
		col = (LIGHT_ON * 0xFFFFFF) + (!LIGHT_ON * draw->light_color[nb_hit]);
		td->info.light_color[nb_hit].r[td->current_pix] = (col >> 16) & 0xFF;
		td->info.light_color[nb_hit].g[td->current_pix] = (col >> 8) & 0xFF;
		td->info.light_color[nb_hit].b[td->current_pix] = col & 0xFF;
		td->info.refl_val[nb_hit][td->current_pix] = draw->reflectance[nb_hit];
		td->info.emittance[nb_hit][td->current_pix] = (LIGHT_ON * 1)
			+ (!LIGHT_ON * draw->light_emit[nb_hit]);
		draw->tex_pos_fp[nb_hit] += draw->step_fp[nb_hit];
	}
}

t_vec	reflect_across_mirror(t_vec point, t_draw *draw, t_point curr,
	int *bounce)
{
	t_vec	normal;
	t_vec	hit;
	int		b;
	float	dot;

	b = -1;
	while (++b < draw->bounces)
	{
		if (curr.y < draw->draw_start[b] || curr.y > draw->draw_end[b])
			break ;
		normal = draw->normal[b];
		hit = draw->hit[b];
		dot = 2 * ((point.x - hit.x) * normal.x + (point.y - hit.y) * normal.y);
		point.x -= dot * normal.x;
		point.y -= dot * normal.y;
	}
	*bounce = b;
	return (point);
}

void	draw_ceil(t_data *data, t_th_draw *td, t_point curr, t_draw *draw)
{
	t_img		*tex;
	t_flight	light;
	t_vec		cast;
	t_vec		world;
	t_point		i;

	cast = td->ray_dir.cast_table[curr.y * data->render_w + curr.x];
	world = (t_vec){data->map->player.x + cast.y * (td->ray_dir.l.x + cast.x
			* td->ray_dir.r.x), data->map->player.y + cast.y
		* (td->ray_dir.l.y + cast.x * td->ray_dir.r.y)};
	world = reflect_across_mirror(world, draw, curr, &td->bounces);
	if (world.x < 0 || world.x >= data->map->wid || world.y < 0
		|| world.y >= data->map->len)
		return (setup_color(draw, td, (t_col){0}, 0));
	i = (t_point){(int)world.x, (int)world.y};
	if (!td->ray_dir.tile_dict[data->map->map[i.y * data->map->wid + i.x]])
		return (setup_color(draw, td, (t_col){0}, 0));
	tex = td->ray_dir.tile_dict[data->map->map[i.y * data->map->wid
		+ i.x]]->tex_ce.img;
	light = data->lmap.lmap[(int)(world.x * LMAP_PRECISION)
		+ data->lmap.wid * (int)(world.y * LMAP_PRECISION)].ce_fl;
	setup_color(draw, td, color_blend(*(tex->data + tex->size_line
				* (int)((world.y - i.y) * tex->height) + (int)((world.x - i.x)
					* tex->width)), light.color, light.emittance), td->bounces);
}

void	draw_floor(t_data *data, t_th_draw *td, t_point curr, t_draw *draw)
{
	t_img		*tex;
	t_flight	light;
	t_vec		cast;
	t_vec		world;
	t_point		i;

	cast = td->ray_dir.cast_table[curr.y * data->render_w + curr.x];
	world = (t_vec){data->map->player.x + cast.y * (td->ray_dir.l.x + cast.x
			* td->ray_dir.r.x), data->map->player.y + cast.y
		* (td->ray_dir.l.y + cast.x * td->ray_dir.r.y)};
	world = reflect_across_mirror(world, draw, curr, &td->bounces);
	if (world.x < 0 || world.x >= data->map->wid || world.y < 0
		|| world.y >= data->map->len)
		return (setup_color(draw, td, (t_col){0}, 0));
	i = (t_point){(int)world.x, (int)world.y};
	if (!td->ray_dir.tile_dict[data->map->map[i.y * data->map->wid + i.x]])
		return (setup_color(draw, td, (t_col){0}, 0));
	tex = td->ray_dir.tile_dict[data->map->map[i.y * data->map->wid
		+ i.x]]->tex_fl.img;
	light = data->lmap.lmap[(int)(world.x * LMAP_PRECISION)
		+ data->lmap.wid * (int)(world.y * LMAP_PRECISION)].ce_fl;
	setup_color(draw, td, color_blend(*(tex->data + tex->size_line
				* (int)((world.y - i.y) * tex->height) + (int)((world.x - i.x)
					* tex->width)), light.color, light.emittance), td->bounces);
}

void	draw_wall(t_th_draw *td, t_draw *draw)
{
	draw->tex_pos_fp[draw->bounces] += draw->step_fp[draw->bounces];
	setup_color(draw, td, color_blend(*(int *)(draw->tex_col[draw->bounces]
				+ (draw->tex_pos_fp[draw->bounces] >> 16) * draw->tex_sizeline
			[draw->bounces]), draw->light_color[draw->bounces],
			draw->light_emit[draw->bounces]), draw->bounces);
}
