/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   put_pixel.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 16:24:55 by hle-hena          #+#    #+#             */
/*   Updated: 2025/09/11 14:50:27 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D_bonus.h"

inline void	ft_put_pixel(t_data *data, t_point point, int color)
{
	int	*dst;

	dst = data->img[data->img_buffer].data + point.y * data->img[0].size_line
		+ point.x;
	*dst = color;
}
