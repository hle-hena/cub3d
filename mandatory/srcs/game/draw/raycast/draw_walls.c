/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_walls.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 11:17:14 by hle-hena          #+#    #+#             */
/*   Updated: 2025/08/29 11:45:59 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static inline void	draw_floor(t_point curr, t_rdir ray, char *img,
	int is_floor)
{
	t_data		*data;
	t_img		*tex;
	t_vec		cast;
	t_vec		world;
	t_point		i;

	data = get_data();
	cast = ray.cast_table[curr.y * data->win_wid + curr.x];
	world = (t_vec){data->map->player.x + cast.y * (ray.l.x + cast.x
			* ray.r.x), data->map->player.y + cast.y
		* (ray.l.y + cast.x * ray.r.y)};
	if (world.x < 0 || world.x >= data->map->wid || world.y < 0
		|| world.y >= data->map->len)
		return ((void)(*(int *)img = 0));
	i = (t_point){(int)world.x, (int)world.y};
	if (!ray.tile_dict[data->map->matrix[i.y * data->map->wid + i.x]])
		return ((void)(*(int *)img = 0));
	tex = ray.tile_dict[data->map->matrix[i.y * data->map->wid + i.x]]->tex_ce;
	if (is_floor)
		tex = ray.tile_dict[data->map->matrix[i.y * data->map->wid
			+ i.x]]->tex_fl;
	*(int *)img = *(((int *)tex->data) + (tex->size_line / 4)
			* (int)((world.y - i.y) * tex->height) + (int)((world.x - i.x)
				* tex->width));
}

static inline void	draw_wall(t_hit *hit, char *img)
{
	hit->tex_y = hit->tex_pos_fp >> 16;
	*(int *)img = *(int *)(hit->tex_col + hit->tex_y
			* hit->texture->size_line);
	hit->tex_pos_fp += hit->step_fp;
}

void	*draw_walls(void *arg)
{
	t_th_draw	*td;
	t_data		*data;
	t_hit		*hit;
	char		*img;
	t_point		curr;

	td = (t_th_draw *)arg;
	data = get_data();
	img = data->img.data + td->start_x * data->img.bpp;
	curr.y = -1;
	while (++curr.y < data->win_len)
	{
		curr.x = td->start_x - 1;
		while (++curr.x < td->end_x)
		{
			hit = &data->hits[curr.x];
			if (curr.y >= hit->draw_start && curr.y < hit->draw_end)
				draw_wall(hit, img);
			else
				draw_floor(curr, td->ray_dir, img, curr.y > hit->draw_start);
			img += data->img.bpp;
		}
		img += td->add_next_line;
	}
	return (NULL);
}
