/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 22:06:06 by hle-hena          #+#    #+#             */
/*   Updated: 2025/08/29 11:46:10 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	draw(t_data *data)
{
	t_th_draw	td;
	t_rdir		ray_dir;
	int			i;

	i = -1;
	ray_dir = (t_rdir){(t_vec){data->cam.dir.x - data->cam.plane.x,
		data->cam.dir.y - data->cam.plane.y}, (t_vec){data->cam.plane.x * 2,
		data->cam.plane.y * 2}, *get_cast_table(), get_tile_dict()};
	td.start_x = 0;
	td.end_x = data->win_wid;
	td.add_next_line = td.start_x * data->img.bpp;
	td.ray_dir = ray_dir;
	draw_walls(&td);
}

t_hit	cast_ray(t_data *data, t_vec ray_dir)
{
	t_hit	ray_hit;
	float	wall_x;
	int		line_height;
	int		tex_start;
	int		tex_end;

	ray_hit = raycast(data, ray_dir, data->map->player);
	ray_hit.ray_dir = ray_dir;
	ray_hit.dist = fmax(ray_hit.dist, 0.0001f);
	line_height = fmax(1, (int)(data->win_len * 2 / ray_hit.dist));
	tex_start = -line_height / 2 + data->win_len / 2;
	tex_end = line_height / 2 + data->win_len / 2;
	ray_hit.draw_start = ft_max(tex_start, 0);
	ray_hit.draw_end = ft_min(tex_end, data->win_len - 1);
	wall_x = ray_hit.ray_hit.x;
	if (ray_hit.side == 0)
		wall_x = ray_hit.ray_hit.y;
	wall_x -= (int)wall_x;
	line_height = tex_end - tex_start;
	ray_hit.step_fp = (ray_hit.texture->height << 16) / line_height;
	ray_hit.tex_pos_fp = (ray_hit.draw_start - data->win_len / 2 + line_height
			/ 2) * ray_hit.step_fp;
	ray_hit.tex_col = ray_hit.texture->data + (int)(wall_x
			* ray_hit.texture->width) * ray_hit.texture->bpp;
	return (ray_hit);
}

void	cast_rays(t_data *data)
{
	int		x;
	float	cam_x;
	t_vec	ray_dir;
	float	look_dir;

	look_dir = (float)data->map->player.rot * PI / 180;
	data->cam.dir = (t_vec){cos(look_dir), sin(look_dir)};
	data->cam.plane = (t_vec){-sin(look_dir) * tan((float)(60 * PI / 180) / 2),
		cos(look_dir) * tan((float)(60 * PI / 180) / 2)};
	x = -1;
	while (++x < data->win_wid)
	{
		cam_x = 2.0f * x / data->win_wid - 1.0f;
		ray_dir.x = data->cam.dir.x + data->cam.plane.x * cam_x;
		ray_dir.y = data->cam.dir.y + data->cam.plane.y * cam_x;
		data->hits[x] = cast_ray(data, ray_dir);
	}
	draw(data);
}
