/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 21:48:07 by hle-hena          #+#    #+#             */
/*   Updated: 2025/08/28 10:11:10 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

int	mouse_move(int x, int y, t_data *data)
{
	static int	mouse_pos = -1;

	if (mouse_pos == -1)
		mouse_pos = x;
	data->event.rot = x - mouse_pos;
	if (x <= 0)
		mlx_mouse_move(data->mlx, data->win, data->win_w - 2, y);
	else if (x >= data->win_w - 1)
		mlx_mouse_move(data->mlx, data->win, 1, y);
	mouse_pos = x;
	return (0);
}
