/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 16:05:37 by hle-hena          #+#    #+#             */
/*   Updated: 2025/04/18 11:47:01 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

t_data	*get_data(void)
{
	static t_data	data = (t_data){0};

	return (&data);
}

void	init_img_stripes(t_data *data)
{
	int		i;
	int		nb_stripes;
	t_c_img	*stripes;

	nb_stripes = ceil((float)data->win_wid / STRIPE_LEN);
	stripes = malloc(nb_stripes * sizeof(t_c_img));
	if (!stripes)
		ft_perror(1, "cub3d: Internal error: malloc.", clean_data());
	i = -1;
	while (++i < nb_stripes - 1)
	{
		stripes[i].img = mlx_new_image(data->mlx, STRIPE_LEN, data->win_len);
		stripes[i].data = mlx_get_data_addr(stripes[i].img, &stripes[i].bpp,
			&stripes[i].size_line, &stripes[i].endian);
		stripes[i].bpp /= 8;
	}
	nb_stripes = ft_tern_int(data->win_wid % STRIPE_LEN, data->win_wid % STRIPE_LEN, STRIPE_LEN);
	stripes[i].img = mlx_new_image(data->mlx, nb_stripes, data->win_len);
	stripes[i].data = mlx_get_data_addr(stripes[i].img, &stripes[i].bpp,
		&stripes[i].size_line, &stripes[i].endian);
	stripes[i].bpp /= 8;
	data->img_stripes = stripes;
}

void	init_mlx(t_data *data)
{
	mlx_get_screen_size(data->mlx, &data->win_wid, &data->win_len);
	data->win_len *= 0.9;
	printf("height : %d, len : %d\n\n", data->win_len, data->win_wid);
	data->win = mlx_new_window(data->mlx, data->win_wid, data->win_len,
		"Cub3d");
	data->event = (t_event){0};
}

void	init_utils(t_data *data)
{
	data->map->mini_map = (t_point)
		{data->win_wid * 0.9, data->win_len * 0.15, 0};
	data->map->mini_map_scale = 32;
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
	init_img_stripes(data);
	loop();
	return (0);
}
