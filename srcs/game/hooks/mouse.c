/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 21:48:07 by hle-hena          #+#    #+#             */
/*   Updated: 2025/04/23 15:37:28 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	mouse_down(int button, int x, int y, t_data *data)
{
	(void)x;
	(void)y;
	(void)data;
	printf("Mouse down : [%d]\n", button);
	return (0);
}

int	mouse_up(int button, int x, int y, t_data *data)
{
	(void)x;
	(void)y;
	(void)data;
	printf("Mouse up : [%d]\n", button);
	return (0);
}

int	mouse_move(int x, int y, t_data *data)
{
	static int	mouse_pos = -1;

	if (mouse_pos == -1)
		mouse_pos = x;
	data->event.rot = x - mouse_pos;
	if (x <= 0)
		mlx_mouse_move(data->mlx, data->win, data->win_wid - 2, y);
	else if (x >= data->win_wid - 1)
		mlx_mouse_move(data->mlx, data->win, 1, y);
	mouse_pos = x;
	return (0);
}
