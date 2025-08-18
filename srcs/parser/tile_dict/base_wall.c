/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   base_wall.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 10:08:05 by hle-hena          #+#    #+#             */
/*   Updated: 2025/08/18 12:09:34 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	add_wall(t_tile *tile, t_wpath content, int *err)
{
	t_wpath	*wpath;

	wpath = malloc(sizeof(t_wpath));
	if (!wpath)
	{
		*err = 1;
		ft_perror(-1, "Internal error: malloc.", 0);
		return (1);
	}
	*wpath = content;
	add_link(&tile->wpath, wpath);
	return (0);
}

void	add_base_wall(t_tile *tile, int *err)
{
	if (tile->wpath)
		return ;
	if (add_wall(tile, (t_wpath){(t_vec){0, 1}, (t_vec){0, 0}, (t_vec){0},
			tile->tex_we, (t_vec){-1, 0}, tile->tex_we.reflectance, 0, 0,
			1, 0, 1}, err))
		return ;
	if (add_wall(tile, (t_wpath){(t_vec){0, 0}, (t_vec){1, 0}, (t_vec){0},
			tile->tex_no, (t_vec){0, -1}, tile->tex_no.reflectance, 0, 0,
			1, 0, 1}, err))
		return ;
	if (add_wall(tile, (t_wpath){(t_vec){1, 1}, (t_vec){0, 1}, (t_vec){0},
			tile->tex_so, (t_vec){0, 1}, tile->tex_so.reflectance, 0, 0,
			1, 0, 1}, err))
		return ;
	if (add_wall(tile, (t_wpath){(t_vec){1, 0}, (t_vec){1, 1}, (t_vec){0},
			tile->tex_ea, (t_vec){1, 0}, tile->tex_ea.reflectance, 0, 0,
			1, 0, 1}, err))
		return ;
}
