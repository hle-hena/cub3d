/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 16:05:37 by hle-hena          #+#    #+#             */
/*   Updated: 2025/05/27 15:44:59 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

t_data	*get_data(void)
{
	static t_data	data = (t_data){0};

	return (&data);
}

void	init_mlx(t_data *data)
{
	data->empty = ft_calloc(1, sizeof(t_void));
	data->empty->flight = ft_calloc(1, sizeof(t_flight));
	data->empty->texture.img = malloc(sizeof(t_img));
	data->empty->texture.img = mlx_new_image(data->mlx, 1, 1);
	data->empty->texture.img->data = mlx_get_data_addr(data->empty->texture.img,
		&data->empty->texture.img->bpp, &data->empty->texture.img->size_line,
		&data->empty->texture.img->endian);
	data->empty->texture.img->bpp /= 8;
	mlx_get_screen_size(data->mlx, &data->render_w, &data->render_h);
	data->render_h *= 0.9;
	// data->render_h *= 2;
	// data->render_w *= 2;
	data->render_h += (data->render_h % 2);
	data->render_w += (data->render_w % 2);
	data->win_h = data->render_h;
	data->win_w = data->render_w;
	data->render_h /= 2;
	data->render_w /= 2;
	data->win = mlx_new_window(data->mlx, data->win_w, data->win_h,
		"Cub3d");
	data->img.img = mlx_new_image(data->mlx, data->win_w, data->win_h);
	data->img.data = mlx_get_data_addr(data->img.img, &data->img.bpp,
		&data->img.size_line, &data->img.endian);
	data->img.bpp /= 8;
	data->event = (t_event){0};
	data->delta_t = 0;
	data->lmap.nb_ls = 0;
	data->lmap.lights = NULL;
}

void	init_utils(t_data *data)
{
	int	err;

	err = 0;
	data->map->mini_map = (t_point)
		{data->win_w * 0.9, data->win_h * 0.15};
	data->map->mini_map_scale = 32;
	data->hits = malloc(data->render_w * sizeof(t_hit));
	if (!data->hits)
		ft_perror(1, "cub3d: Internal error: malloc.", clean_data());
	fill_cast_table(data, &err);
	if (err)
		ft_perror(1, NULL, 0);
}

int	main(int ac, char **av)
{
	t_data	*data;

	data = get_data();
	data->mlx = mlx_init();
	// mlx_do_key_autorepeatoff(data->mlx);
	data->map = load_map(ac, av);
	if (data->map)
	{
		if (create_lmap(data))
		{
			clean_data();
			return (0);
		}
		// print_dict(data);
		init_mlx(data);
		init_utils(data);
		loop();
	}
	clean_data();
	return (0);
}
