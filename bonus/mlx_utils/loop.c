/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 21:36:26 by hle-hena          #+#    #+#             */
/*   Updated: 2025/09/11 15:23:40 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D_bonus.h"

int	mlx_close(t_data *data)
{
	(void)data;
	clean_data();
	printf("\nEnd of cub3D\n");
	exit(0);
}

void	loop(void)
{
	t_data	*data;

	data = get_data();
	mlx_hook(data->win, 2, 1L << 0, key_down, data);
	mlx_hook(data->win, 3, 1L << 1, key_up, data);
	mlx_hook(data->win, 6, 1L << 6, mouse_move, data);
	mlx_hook(data->win, 17, 0, mlx_close, data);
	mlx_loop_hook(data->mlx, event_loop, data);
	mlx_loop(data->mlx);
}
