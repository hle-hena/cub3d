/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 16:05:37 by hle-hena          #+#    #+#             */
/*   Updated: 2025/04/18 23:04:07 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

t_data	*get_data(void)
{
	static t_data	data = (t_data){0};

	return (&data);
}

void	init_th_img(t_data *data)
{
	int	i;
	int	img_wid;
	int	slice;
	int	width_left;

	i = -1;
	data->th_img = malloc(DRAW_THREADS * sizeof(t_img));
	if (!data->th_img)
		ft_perror(1, "cub3d: Internal error: malloc", clean_data());
	slice = data->win_wid / DRAW_THREADS;
	width_left = data->win_wid;
	while (++i < DRAW_THREADS)
	{
		img_wid = ft_min(slice, width_left);
		width_left -= img_wid;
		data->th_img[i].img = mlx_new_image(data->mlx, img_wid, data->win_len);
		data->th_img[i].data = mlx_get_data_addr(data->th_img[i].img,
			&data->th_img[i].bpp, &data->th_img[i].size_line,
			&data->th_img[i].endian);
		data->th_img[i].bpp /= 8;
	}
}

void	init_mlx(t_data *data)
{
	mlx_get_screen_size(data->mlx, &data->win_wid, &data->win_len);
	data->win_len *= 0.9;
	// data->win_len /= 2;
	// data->win_wid /= 2;
	printf("Win wid is %d and height is %d\n", data->win_wid, data->win_len);
	data->win = mlx_new_window(data->mlx, data->win_wid, data->win_len,
		"Cub3d");
	data->img.img = mlx_new_image(data->mlx, data->win_wid, data->win_len);
	data->img.data = mlx_get_data_addr(data->img.img, &data->img.bpp,
		&data->img.size_line, &data->img.endian);
	data->img.bpp /= 8;
	data->event = (t_event){0};
	init_th_img(data);
}

void	init_utils(t_data *data)
{
	data->map->mini_map = (t_point)
		{data->win_wid * 0.9, data->win_len * 0.15, 0};
	data->map->mini_map_scale = 32;
	data->hits = malloc(data->win_wid * sizeof(t_hit));
	if (!data->hits)
		ft_perror(-1, "cub3d: Internal error: malloc.", clean_data());
}

int	main(int ac, char **av)
{
	t_data	*data;

	data = get_data();
	data->mlx = mlx_init();
	// mlx_do_key_autorepeatoff(data->mlx);
	data->map = load_map(ac, av);
	init_mlx(data);
	init_utils(data);
	loop();
	return (0);
}
