/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 22:06:06 by hle-hena          #+#    #+#             */
/*   Updated: 2025/04/16 15:05:28 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void draw_wall(t_data *data, t_hit ray_hit, t_point limit, int x)
{
	int		y;
	int		tex_x;
	int		tex_y;
	float	wall_x;
	char	*row;

	if (ray_hit.side == 0)
		wall_x = ray_hit.ray_hit.y - floor(ray_hit.ray_hit.y);
	else
		wall_x = ray_hit.ray_hit.x - floor(ray_hit.ray_hit.x);
	if ((ray_hit.side == 0 && ray_hit.ray_dir.x > 0)
		|| (ray_hit.side == 1 && ray_hit.ray_dir.y < 0))
		wall_x = 1.0f - wall_x;
	tex_x = (int)(wall_x * (float)ray_hit.texture.width);
	if (tex_x < 0)
		tex_x = 0;
	if (tex_x >= ray_hit.texture.width)
		tex_x = ray_hit.texture.width - 1;
	y = ft_max(limit.x - 1, 0);
	row = data->img.data + y * data->img.size_line + x * data->img.bpp;
	int line_height = limit.y - limit.x;
	int	draw_end = ft_min(limit.y, data->win_len);
	float step = (float)ray_hit.texture.height / line_height;
	float tex_pos = (ft_max(limit.x, 0) - data->win_len / 2 + line_height / 2) * step;
	while (++y < draw_end)
	{
		tex_y = (int)tex_pos;
		if (tex_y < 0)
			tex_y = 0;
		else if (tex_y >= ray_hit.texture.height)
			tex_y = ray_hit.texture.height - 1;
		*(int *)row = *(int *)(ray_hit.texture.data + tex_y * ray_hit.texture.size_line + tex_x * ray_hit.texture.bpp);
		row += data->img.size_line;
		tex_pos += step;
	}
}

	// struct timespec start, end;
	// clock_gettime(CLOCK_MONOTONIC, &start);
	// CODE TO BENCHMARK
	// clock_gettime(CLOCK_MONOTONIC, &end);
	// long ms = (end.tv_sec - start.tv_sec) * 1000000000L + (end.tv_nsec - start.tv_nsec);
	// printf("Took %ldns\n", ms);

void cast_ray(t_data *data, t_vec ray_dir, float ray_angle, int x)
{
	t_hit	ray_hit;
	int		line_height;
	int		draw_start;
	int		draw_end;

	ray_hit = raycast(data, ray_dir, data->map->player);
	ray_hit.ray_dir = ray_dir;
	ray_hit.dist = ray_hit.dist * cos(ray_angle - data->map->player.rot * PI / 180);
	if (ray_hit.dist <= 0.0f)
		ray_hit.dist = 0.0001f;
	line_height = (int)(data->win_len / ray_hit.dist);
	draw_start = -line_height / 2 + data->win_len / 2;
	draw_end = line_height / 2 + data->win_len / 2;
	draw_wall(data, ray_hit, (t_point){draw_start, draw_end, 0}, x);
}

void cast_rays(t_data *data)
{
	int		x;
	float	cam_x;
	float	ray_angle;
	float	fov;
	t_vec	ray_dir;

	x = -1;
	fov = 60 * PI / 180;
	while (++x < data->win_wid)
	{
		cam_x = (2.0f * x) / data->win_wid - 1.0f;
		ray_angle = data->map->player.rot * (PI / 180.0f)
			+ atan(cam_x * tan(fov / 2.0f));
		ray_dir.x = cos(ray_angle);
		ray_dir.y = sin(ray_angle);
		cast_ray(data, ray_dir, ray_angle, x);
	}
}
