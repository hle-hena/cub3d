/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 21:43:05 by hle-hena          #+#    #+#             */
/*   Updated: 2025/08/29 11:29:48 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	key_down(int keycode, t_data *data)
{
	if (keycode == 65307)
		data->event.echap = 1;
	else if (keycode == 119)
		data->event.mo_f = 1;
	else if (keycode == 115)
		data->event.mo_b = -1;
	else if (keycode == 97)
		data->event.mo_l = -1;
	else if (keycode == 100)
		data->event.mo_r = 1;
	return (0);
}

int	key_up(int keycode, t_data *data)
{
	if (keycode == 119)
		data->event.mo_f = 0;
	else if (keycode == 115)
		data->event.mo_b = 0;
	else if (keycode == 97)
		data->event.mo_l = 0;
	else if (keycode == 100)
		data->event.mo_r = 0;
	return (0);
}
