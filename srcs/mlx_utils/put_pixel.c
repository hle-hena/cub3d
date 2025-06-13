/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   put_pixel.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 16:24:55 by hle-hena          #+#    #+#             */
/*   Updated: 2025/06/12 15:29:10 by hle-hena         ###   ########.fr       */
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

int	ft_get_pixel_color(t_data *data, t_point point)
{
	int	*img;
	int	temp;
	int	size_line;

	img = (int *)mlx_get_data_addr(data->img.img, &temp, &size_line, &temp);
	return (img[point.y * data->win_w + point.x]);
}

inline void	ft_put_pixel(t_data *data, t_point point, int color)
{
	int *dst = data->img.data + point.y * data->img.size_line
		+ point.x;
	*dst = color;
}
