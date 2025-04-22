/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 16:05:37 by hle-hena          #+#    #+#             */
/*   Updated: 2025/04/22 11:42:47 by hle-hena         ###   ########.fr       */
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
	mlx_get_screen_size(data->mlx, &data->win_wid, &data->win_len);
	data->win_len *= 0.9;
	// data->win_wid *= 2;
	// data->win_len *= 2;
	data->win = mlx_new_window(data->mlx, data->win_wid, data->win_len,
		"Cub3d");
	data->img.img = mlx_new_image(data->mlx, data->win_wid, data->win_len);
	data->img.data = mlx_get_data_addr(data->img.img, &data->img.bpp,
		&data->img.size_line, &data->img.endian);
	data->img.bpp /= 8;
	data->event = (t_event){0};
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
	if (data->map)
	{
		print_dict(data);	
		init_mlx(data);
		init_utils(data);
		loop();
	}
	clean_data();
	return (0);
}
