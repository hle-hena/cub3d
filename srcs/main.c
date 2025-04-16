/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 16:05:37 by hle-hena          #+#    #+#             */
/*   Updated: 2025/04/16 16:55:11 by hle-hena         ###   ########.fr       */
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
	int			i;
	int			nb_stripes;
	t_stripe	*stripes;

	nb_stripes = ceil((float)data->win_wid / STRIPE_LEN);
	stripes = malloc(nb_stripes * sizeof(t_stripe));
	if (!stripes)
		ft_perror(1, "cub3d: Internal error: malloc.", clean_data());
	i = -1;
	while (++i < nb_stripes - 1)
	{
		stripes[i].data = malloc((STRIPE_LEN * data->win_len) * sizeof(int));
		if (!stripes[i].data)
		{
			ft_free_tab((void **)stripes, i - 1);
			ft_perror(1, "cub3d: Internal error: malloc.", clean_data());
		}
		stripes[i].size_line = STRIPE_LEN;
	}
	nb_stripes = ft_tern_int(data->win_wid % STRIPE_LEN, data->win_wid % STRIPE_LEN, STRIPE_LEN);
	stripes[i].data = malloc((nb_stripes * data->win_len) * sizeof(int));
	if (!stripes[i].data)
	{
		ft_free_tab((void **)stripes, i - 1);
		ft_perror(1, "cub3d: Internal error: malloc.", clean_data());
	}
	stripes[i].size_line = nb_stripes;
	data->img_stripes = stripes;
}

void	init_mlx(t_data *data)
{
	mlx_get_screen_size(data->mlx, &data->win_wid, &data->win_len);
	data->win_len *= 0.9;
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
