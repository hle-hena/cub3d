/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   put_pixel.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 16:24:55 by hle-hena          #+#    #+#             */
/*   Updated: 2025/06/17 15:05:31 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	point_is_in_fov(t_data *data, t_point point)
{
	if (point.x < data->win_w && point.x >= 0
		&& point.y < data->win_h
		&& point.y >= 0)
		return (1);
	return (0);
}

inline void	ft_put_pixel(t_data *data, t_point point, int color)
{
	int *dst = data->img[data->img_buffer].data + point.y * data->img[0].size_line
		+ point.x;
	*dst = color;
}
