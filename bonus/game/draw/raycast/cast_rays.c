/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cast_rays.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 11:38:49 by hle-hena          #+#    #+#             */
/*   Updated: 2025/09/11 14:51:14 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D_bonus.h"

void	init_draw_info_i(t_data *data, t_draw *draw, t_hit hit, int i)
{
	int	line_height;
	int	tex_start;
	int	tex_end;

	if (hit.dist[i] <= 0.0f)
		hit.dist[i] = 0.0001f;
	line_height = ft_max((int)(data->render_h * 2 / hit.dist[i]), 1);
	tex_start = -line_height / 2 + data->render_h / 2;
	tex_end = (float)line_height / 2 + data->render_h / 2
		+ (hit.wall[i].reflectance < FLT_EPSILON);
	draw->draw_start[i] = ft_max(tex_start, 0);
	draw->draw_end[i] = ft_min(tex_end, data->render_h);
	line_height = ft_max(tex_end - tex_start, 1);
	draw->step_fp[i] = (hit.texture[i]->height << 16) / line_height;
	draw->tex_pos_fp[i] = (draw->draw_start[i]
			- data->render_h / 2 + line_height / 2) * draw->step_fp[i];
	draw->tex_col[i] = hit.texture[i]->data
		+ (int)(hit.wall[i].pos * hit.texture[i]->width);
	draw->tex_sizeline[i] = hit.texture[i]->size_line;
	draw->light_color[i] = hit.light[i]->color;
	draw->light_emit[i] = hit.light[i]->emittance;
	draw->reflectance[i] = hit.wall[i].reflectance;
	draw->normal[i] = hit.wall[i].normal;
	draw->hit[i] = hit.hit[i];
}

t_draw	cast_ray(t_data *data, t_vec ray_dir)
{
	t_hit	hit;
	t_draw	draw;
	int		i;

	hit = raycast(data, ray_dir, (t_vec){data->map->player.x,
			data->map->player.y});
	i = -1;
	draw.bounces = hit.bounces;
	while (++i <= hit.bounces)
		init_draw_info_i(data, &draw, hit, i);
	return (draw);
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
	while (++x < data->render_w)
	{
		cam_x = 2.0f * x / data->render_w - 1.0f;
		ray_dir.x = data->cam.dir.x + data->cam.plane.x * cam_x;
		ray_dir.y = data->cam.dir.y + data->cam.plane.y * cam_x;
		data->draw[x] = cast_ray(data, ray_dir);
	}
	start_threads(data);
}
