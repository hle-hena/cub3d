/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 15:54:38 by hle-hena          #+#    #+#             */
/*   Updated: 2025/04/07 15:39:29 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

#include "libft.h"
# include <math.h>
# include <mlx.h>
# include <fcntl.h>
# include <time.h>
# include <stdio.h>

typedef struct s_trigometry_values
{
	float	sin;
	float	cos;
}	t_trig;

typedef struct s_color
{
	int re;
	int gr;
	int bl;
}	t_col;


typedef struct s_vector
{
	float	x;
	float	y;
	float	z;
}	t_vec;

typedef struct s_quat
{
	float	w;
	float	x;
	float	y;
	float	z;
}	t_quat;

typedef struct s_point
{
	int		x;
	int		y;
	int		z;
}	t_point;

typedef struct s_img
{
	void	*img;
	int		width;
	int		height;
}	t_img;

typedef struct s_map
{
	int		**matrix;
	int		len;
	int		wid;
	t_img	img_no;
	t_img	img_so;
	t_img	img_we;
	t_img	img_ea;
	t_col	floor;
	t_col	ceiling;
}	t_map;

typedef struct s_events
{
	int		mo_f;
	int		mo_s;
	int		rot_y;
	int		lock;
}	t_event;

typedef struct s_data
{
	void	*mlx;
	void	*win;
	void	*img;
	int		win_len;
	int		win_wid;
	t_map	*map;
	t_event	event;
}	t_data;


t_data	*get_data(void);
t_map	*get_map(void);
t_map	load_map(int ac, char **av);
void	retrieve_map_info(t_map **map, char *map_name, int *err);


#endif