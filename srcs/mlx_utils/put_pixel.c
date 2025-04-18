/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   put_pixel.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 16:24:55 by hle-hena          #+#    #+#             */
/*   Updated: 2025/04/18 10:31:29 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	point_is_in_fov(t_data *data, t_point point)
{
	if (point.x < data->win_wid && point.x >= 0
		&& point.y < data->win_len
		&& point.y >= 0)
		return (1);
	return (0);
}

inline void	ft_put_pixel(t_data *data, t_point point, int color)
{
	int		i;
	char	*dst;

	i = point.x / STRIPE_LEN;
	dst = data->img_stripes[i].data + point.y * data->img_stripes[i].size_line
		+ (point.x % STRIPE_LEN) * data->img_stripes[i].bpp;
	*(int *)dst = color;
}
