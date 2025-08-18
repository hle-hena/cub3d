/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_image.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 16:21:30 by hle-hena          #+#    #+#             */
/*   Updated: 2025/08/07 16:36:50 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	new_image(t_data *data, t_img *img_struct, int width, int height)
{
	img_struct->img = mlx_new_image(data->mlx, width, height);
	if (!img_struct->img)
		ft_perror(1, "An mlx image didnt open.", clean_data());
	img_struct->data = (int *)mlx_get_data_addr(img_struct->img,
			&img_struct->bpp, &img_struct->size_line, &img_struct->endian);
	img_struct->bpp /= 8;
	img_struct->size_line /= img_struct->bpp;
}
