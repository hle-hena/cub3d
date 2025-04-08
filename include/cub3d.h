/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 15:54:38 by hle-hena          #+#    #+#             */
/*   Updated: 2025/04/08 17:21:27 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include "libft.h"
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

typedef struct s_flood
{
	t_point			point;
	struct s_list	*next;
}	t_flood;


typedef struct s_img
{
	void	*img;
	int		width;
	int		height;
}	t_img;

typedef struct s_player
{
	float	x;
	float	y;
}	t_player;

typedef struct s_map
{
	char		**matrix;
	int			len;
	int			wid;
	t_img		img_no;
	t_img		img_so;
	t_img		img_we;
	t_img		img_ea;
	t_col		floor;
	t_col		ceiling;
	t_player	player;
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
t_map	*load_map(int ac, char **av);
void	retrieve_map_info(t_map *map, int map_fd, int *err);
int		ft_atoi_err(char *arg, int *index);
int		has_all_info(t_map *map);
int		clean_map(void);
int		clean_data(void);
void	print_missing_info(t_map *map);
t_img	get_path(char *line, int *err);
t_col	get_color(char *line, int *err);
void	add_link(t_list **lst, char *content);
void	retrieve_map_grid(t_map *map, int map_fd, int *err);

#endif