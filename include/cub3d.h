/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 15:54:38 by hle-hena          #+#    #+#             */
/*   Updated: 2025/07/21 17:15:33 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H

# define CUB3D_H

# include "libft.h"
# include <math.h>
# include <mlx.h>
# include <fcntl.h>
# include <time.h>
# include <sys/time.h>
# include <stdio.h>
# include <pthread.h>
# include <immintrin.h>

# define FLT_EPSILON 1.19209289550781250000000000000000000e-7F
# define PI 3.1415926535897932384
# define VOID (void)0

typedef unsigned long long	t_uint64;
typedef long long			t_int64;

# define TARGET_FPS 60
# define FRAME_TIME_US (1000000 / 60)

typedef struct s_trigometry_values
{
	float	sin;
	float	cos;
}	t_trig;

typedef struct s_color
{
	int	re;
	int	gr;
	int	bl;
}	t_col;

typedef struct s_vector
{
	float	x;
	float	y;
}	t_vec;

typedef struct s_point
{
	int		x;
	int		y;
}	t_point;

# define HASH_SIZE 1024

typedef struct s_img
{
	void	*img;
	int		*data;
	char	*path;
	int		bpp;
	int		size_line;
	int		endian;
	int		width;
	int		height;
}	t_img;

typedef struct s_texture
{
	t_img	*img;
	float	reflectance;
}	t_text;

# define MOVE_SPEED 0.075f
# define ROT_SPEED 0.2f

typedef struct s_player
{
	float	x;
	float	y;
	int		rot;
}	t_player;

typedef struct s_trace
{
	t_vec	dist;
	t_vec	slope;
	t_vec	origin;
	t_vec	dir;
	t_point	curr;
	t_point	real;
	t_point	step;
	int		side;
	int		bounce;
	int		running;
	float	precise_dist;
	float	last_dist;
	float	emittance;
	float	base_emittance;
	float	angle_factor;
}	t_trace;

# define ATT_COEF 0.05

typedef struct s_light
{
	t_vec	pos;
	t_col	color;
	float	emittance;
}	t_light;

typedef struct s_light_face
{
	t_vec	normal;
	float	emittance;
	int		color;
}	t_flight;

typedef struct s_light_tile
{
	t_list		*flight;
	t_flight	ce_fl;
}	t_tlight;

typedef struct s_light_map
{
	t_tlight	*lmap;
	t_light		*lights;
	int			nb_ls;
	int			wid;
	int			len;
}	t_lmap;

typedef struct s_intersection
{
	t_vec	coo;
	float	dist;
}	t_inter;

typedef struct s_info_check_intersection
{
	t_inter *inter;
	t_vec	dir;
	float	t;
}	t_info_check;

typedef struct s_graph_link	t_link;
typedef struct s_graph_node	t_node;

struct s_graph_node
{
	t_vec	coo;
	t_link	*connect[512];
	int		visited;
};

struct s_graph_link
{
	t_node	*start;
	t_node	*end;
	t_vec	center;
	int		type;
};

typedef struct s_graph
{
	t_node	nodes[1024];
	t_link	links[512];
	int		nb_links;
	int		nb_nodes;
}	t_graph;

typedef struct s_wpath
{
	t_vec	start;
	t_vec	end;
	t_vec	center;
	t_text	texture;
	t_vec	normal;
	float	reflectance;
	float	pos;
	int		mode;
}	t_wpath;

typedef struct s_tile
{
	t_list		*wpath;
	t_text		tex_no;
	t_text		tex_so;
	t_text		tex_we;
	t_text		tex_ea;
	t_text		tex_ce;
	t_text		tex_fl;
	int			is_wall;
}	t_tile;

typedef struct s_map
{
	int			*matrix;
	char		void_char;
	char		replace_tile;
	int			len;
	int			wid;
	t_player	player;
	t_point		mini_map;
	int			mini_map_scale;
}	t_map;

typedef struct s_events
{
	int		mo_f;
	int		mo_b;
	int		mo_l;
	int		mo_r;
	int		rot;
	int		echap;
}	t_event;

# define MAX_BOUNCE 4

typedef struct s_hit
{
	t_img		*texture[MAX_BOUNCE];
	t_flight	*light[MAX_BOUNCE];
	t_wpath		wall[MAX_BOUNCE];
	t_vec		hit[MAX_BOUNCE];
	float		dist[MAX_BOUNCE];
	int			draw_start[MAX_BOUNCE];
	int			draw_end[MAX_BOUNCE];
	int			tex_pos_fp[MAX_BOUNCE];
	int			step_fp[MAX_BOUNCE];
	int			*tex_col[MAX_BOUNCE];
	int			bounces;
}	t_hit;

typedef struct s_draw
{
	float		reflectance[MAX_BOUNCE];
	int			light_color[MAX_BOUNCE];
	float		light_emittance[MAX_BOUNCE];
	int			tex_pos_fp[MAX_BOUNCE];
	int			tex_sizeline[MAX_BOUNCE];
	int			step_fp[MAX_BOUNCE];
	int			*tex_col[MAX_BOUNCE];
	int			draw_start[MAX_BOUNCE];
	int			draw_end[MAX_BOUNCE];
	t_vec		normal[MAX_BOUNCE];
	t_vec		hit[MAX_BOUNCE];
	int			bounces;
}	t_draw;

typedef struct s_ray
{
	t_vec	dist;
	t_vec	slope;
	t_vec	origin;
	t_vec	dir;
	t_point	curr;
	t_point	step;
	int		start;
	int		side;
	int		bounce;
	int		running;
	float	precise_dist;
}	t_ray;

typedef struct s_ray_dir
{
	t_vec	l;
	t_vec	r;
	t_vec	*cast_table;
	t_tile	**tile_dict;
}	t_rdir;

typedef struct s_cam
{
	t_vec	dir;
	t_vec	plane;
	float	plane_len;
}	t_cam;

typedef struct s_void
{
	t_text		texture;
	t_flight	*flight;
}	t_void;

typedef struct s_col_simd
{
	float	r[8]
		__attribute__((aligned(32)));
	float	g[8]
		__attribute__((aligned(32)));
	float	b[8]
		__attribute__((aligned(32)));
}	t_s_col;

typedef struct s_simd_info
{
	t_s_col										fallback;
	t_s_col										textures[MAX_BOUNCE];
	t_s_col										light_color[MAX_BOUNCE];
	float emittance[MAX_BOUNCE][8]__attribute__	((aligned(32)));
	float refl_val[MAX_BOUNCE][8]__attribute__	((aligned(32)));
	int nb_hit	[8]__attribute__				((aligned(32)));
}	t_simd;

typedef struct s_thread_draw
{
	t_simd			info;
	int				current_pix;
	t_rdir			ray_dir;
	int				start_x;
	int				end_x;
	int				add_next_line;
	int				ready;
	int				done;
	pthread_mutex_t	mutex;
	pthread_cond_t	cond_start;
	pthread_cond_t	cond_done;
}	t_th_draw;

# define IMG_BUFFER 4

typedef struct s_data
{
	void		*mlx;
	void		*win;
	t_img		img[IMG_BUFFER];
	int			win_h;
	int			win_w;
	int			render_h;
	int			render_w;
	float		delta_t;
	t_lmap		lmap;
	t_cam		cam;
	t_map		*map;
	t_draw		*draw;
	t_event		event;
	t_void		*empty;
	int			draw_thread;
	t_th_draw	*thread_pool;
	pthread_t	*threads;
	int			option;
	int			img_buffer;
}	t_data;

# define LMAP_PRECISION 129

int			build_polygons();

void		*draw_walls_thread(void *arg);
t_data		*get_data(void);
t_map		*get_map(void);
t_map		*load_map(int ac, char **av);
int			clean_map(void);
int			clean_data(void);


void		fill_cast_table(t_data *data, int *err);

void		retrieve_line(t_tile *tile, char *arg, int *err);
void		retrieve_arc(t_tile *tile, char *arg, int *err);

t_tile		*new_tile(void);
t_tile		**get_tile_dict(void);
char		*retrieve_tile_dict(t_map *map, int map_fd, int *err);
void		retrieve_tile(t_tile **tiles, int map_fd, char *line, int *err);
void		retrieve_value(int *value, char *arg, int *err, char *id);
void		retrieve_texture(t_text *texture, char *arg, int *err, char *id);
void		retrieve_texture_val(t_text *texture, char *arg, int *err);
void		retrieve_texture_color(t_img **img, char *line, int *err);
t_img		**get_hash_table(void);
t_img		*get_img_hash(char *path, int *err);
void		retrieve_player(t_map *map, char *line, int *err);
int			is_dict_full(t_map *map, int err);
void		retrieve_map(t_map *map, char *line, int map_fd, int *err);
void		retrieve_light(char *line, int *err);
int			retrieve_lonely(t_map *map, char *line, int *err);
int			is_map_valid(t_map *map, t_tile **tiles, int err);

void		add_link(t_list **lst, void *content);

int			is_correct_flight(void *content, void *to_find);
int			does_hit(t_list	*wpath, t_ray *ray, t_wpath *wall);
void		handle_reflexion(t_data *data, t_hit *hit, t_ray *ray,
				t_wpath wall);
void		init_ray(t_ray *ray, t_vec dir, t_vec origin);
t_vec		normalize(t_vec v);
void		handle_hit(t_data *data, t_ray *ray, t_hit *hit);

void		loop(void);
int			mlx_close(t_data *data);
int			event_loop(t_data *data);
int			key_down(int keycode, t_data *data);
int			key_up(int keycode, t_data *data);
int			mouse_down(int button, int x, int y, t_data *data);
int			mouse_up(int button, int x, int y, t_data *data);
int			mouse_move(int x, int y, t_data *data);
t_col		rev_calc_color(int col);
int			calc_color(t_col col);
int			interpolate_color(int col1, int col2, float percent);
int			ft_get_pixel_color(t_data *data, t_point point);

float		ft_atof_err(char *str, float min, float max, char **last);
void		cast_rays(t_data *data);

int			point_is_in_fov(t_data *data, t_point point);
void		draw_player(t_data *data, t_point center, float theta);
void		ft_put_pixel(t_data *data, t_point point, int color);
void		draw_tile(t_data *data, t_point start, t_trig vals, int color);
void		draw_circle(t_data *data, t_point center, int rad, int color);
void		draw_line(t_data *data, t_point start, t_point end, int color);
void		draw_mini_map(t_data *data);
int			point_is_in_mini_map(t_data *data, t_point point);

t_hit		raycast(t_data *data, t_vec dir, t_vec origin);
t_vec		**get_cast_table(void);

int			ft_atoi_err(char *arg, int *index);

void		fps_counter(t_data *data);

int			create_lmap(t_data *data);
void		raytrace(t_data *data, t_light light, t_vec dir);
void		handle_light(t_data *data, t_trace *ray, t_light light);
void		reflect_light(t_data *data, t_trace *ray, t_wpath wall,
				t_light light);
int			does_light(t_list *wpath, t_trace *ray, t_wpath *wall);
void		init_trace(t_trace *ray, t_vec dir, t_vec origin, float emittance);
t_flight	*find_flight(t_data *data, t_point pos, t_wpath wall);

void		print_dict(t_data *data);

#endif