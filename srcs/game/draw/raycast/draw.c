/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 22:06:06 by hle-hena          #+#    #+#             */
/*   Updated: 2025/08/28 10:11:10 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void	set_pixels(t_data *data, int *img, int color)
{
	*(img) = color;
	*(img + 1) = color;
	*(img + data->img[0].size_line) = color;
	*(img + data->img[0].size_line + 1) = color;
}

void	draw_blocked_eight(t_data *data, t_th_draw *td, int **img,
	int *new_line)
{
	int color	[8]__attribute__	((aligned(32)));
	int								i;

	get_colors_simd(td->info, color);
	i = -1;
	while (++i < td->current_pix)
	{
		if (i == *new_line)
		{
			*new_line = -1;
			*img += data->img[0].size_line * 2 - BLOCK_X * 2;
		}
		set_pixels(data, *img, color[i]);
		*img += 2;
	}
	td->current_pix = 0;
}

void	draw_block(t_data *data, t_th_draw *td, t_point bcurr, int *img)
{
	t_point	curr;
	t_draw	*draw;
	int		new_line;

	curr.y = bcurr.y - 1;
	new_line = -1;
	while (++curr.y < bcurr.y + BLOCK_Y && curr.y < data->render_h)
	{
		curr.x = bcurr.x - 1;
		while (++curr.x < bcurr.x + BLOCK_X && curr.x < td->end_x)
		{
			draw = &data->draw[curr.x];
			if (curr.y >= draw->draw_start[draw->bounces]
				&& curr.y < draw->draw_end[draw->bounces])
				draw_wall(td, draw);
			else if (curr.y < draw->draw_start[draw->bounces])
				draw_ceil(data, td, curr, draw);
			else if (curr.y >= draw->draw_end[draw->bounces])
				draw_floor(data, td, curr, draw);
			++td->current_pix;
			if (td->current_pix == 8)
				draw_blocked_eight(data, td, &img, &new_line);
		}
		new_line = td->current_pix;
	}
}

void	draw_walls(t_th_draw *td)
{
	t_data		*data;
	int			*img;
	t_point		bcurr;

	data = get_data();
	td->current_pix = 0;
	bcurr.y = 0;
	while (bcurr.y < data->render_h)
	{
		bcurr.x = td->start_x;
		while (bcurr.x < td->end_x)
		{
			img = data->img[data->img_buffer].data + bcurr.x * 2 + bcurr.y
				* data->img[0].size_line * 2;
			draw_block(data, td, bcurr, img);
			bcurr.x += BLOCK_X;
		}
		bcurr.y += BLOCK_Y;
	}
}
