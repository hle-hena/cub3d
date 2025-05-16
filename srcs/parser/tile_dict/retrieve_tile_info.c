/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   retrieve_tile_info.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 14:28:34 by hle-hena          #+#    #+#             */
/*   Updated: 2025/05/16 09:30:50 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	retrieve_wall(t_tile *tile, char *arg, int *err)
{
	//This func need to be more well done.
	t_line	*line;

	line = malloc(sizeof(t_line));
	if (!line)
		return (*err = 1, VOID);
	arg++;
	arg++;
	line->start.x = ft_atoi(arg);
	while (ft_isdigit(*arg))
		arg++;
	arg++;
	arg++;
	line->start.y = ft_atoi(arg);
	while (ft_isdigit(*arg))
		arg++;
	arg++;
	arg++;
	arg++;
	arg++;
	line->end.x = ft_atoi(arg);
	while (ft_isdigit(*arg))
		arg++;
	arg++;
	arg++;
	line->end.y = ft_atoi(arg);
	add_link(&tile->wpath, line);
}

void	type_switch(t_tile *tile, char *line, char *arg, int *err)
{
	if (ft_strncmp("wl ", line, 3) == 0)
		retrieve_wall(tile, arg, err);
	else if (ft_strncmp("NO ", line, 3) == 0)
		retrieve_texture(&tile->tex_no, arg, err, "NO");
	else if (ft_strncmp("SO ", line, 3) == 0)
		retrieve_texture(&tile->tex_so, arg, err, "SO");
	else if (ft_strncmp("EA ", line, 3) == 0)
		retrieve_texture(&tile->tex_ea, arg, err, "EA");
	else if (ft_strncmp("WE ", line, 3) == 0)
		retrieve_texture(&tile->tex_we, arg, err, "WE");
	else if (ft_strncmp("C ", line, 2) == 0)
		retrieve_texture(&tile->tex_ce, arg, err, "C");
	else if (ft_strncmp("F ", line, 2) == 0)
		retrieve_texture(&tile->tex_fl, arg, err, "F");
	else if (ft_strncmp("W ", line, 2) == 0)
		retrieve_value(&tile->is_wall, arg, err, "W");
	else if (ft_strncmp("CH ", line, 2) == 0)
		retrieve_value(&tile->ceil_height, arg, err, "CH");
	else if (ft_strncmp("FH ", line, 2) == 0)
		retrieve_value(&tile->floor_height, arg, err, "FH");
	else
		(ft_perror(-1, ft_strsjoin((char *[]){"Invalid identifier at \
line : ", line, ".", NULL}), 1), *err = 1);
}

void	retrieve_switch(t_tile *tile, char *line, int *err)
{
	char	*arg;

	if (!line[0] || line[0] == '#')
		return ;
	if (!line[1 + (line[1] != 0)])
		return (ft_perror(-1, ft_strsjoin((char *[]){"Empty line after \
identifier at ", line, ".", NULL}), 1), *err = 1, VOID);
	arg = ft_strtrim(line + 2, " \t\n");
	if (!arg)
		return (*err = 1, VOID);
	type_switch(tile, line, arg, err);
	ft_del((void **)&arg);
}

void	add_wpath(t_tile *tile, int *err)
{
	t_line	*wpath;

	if (tile->wpath)
		return ;
	wpath = malloc(sizeof(t_line));
	if (!wpath)
		return (*err = 1, VOID);
	*wpath = (t_line){(t_vec){0, 0}, (t_vec){0, 1}};
	add_link(&tile->wpath, wpath);
	wpath = malloc(sizeof(t_line));
	if (!wpath)
		return (*err = 1, VOID);
	*wpath = (t_line){(t_vec){0, 0}, (t_vec){1, 0}};
	add_link(&tile->wpath, wpath);
	wpath = malloc(sizeof(t_line));
	if (!wpath)
		return (*err = 1, VOID);
	*wpath = (t_line){(t_vec){1, 1}, (t_vec){0, 1}};
	add_link(&tile->wpath, wpath);
	wpath = malloc(sizeof(t_line));
	if (!wpath)
		return (*err = 1, VOID);
	*wpath = (t_line){(t_vec){1, 1}, (t_vec){1, 0}};
	add_link(&tile->wpath, wpath);
}

t_tile	*new_tile(void)
{
	t_tile	*new;

	new = malloc(sizeof(t_tile));
	if (!new)
		return (NULL);
	new->wpath = NULL;
	new->tex_ce.img = NULL;
	new->tex_ea.img = NULL;
	new->tex_fl.img = NULL;
	new->tex_no.img = NULL;
	new->tex_so.img = NULL;
	new->tex_we.img = NULL;
	new->tex_ce.reflectance = 0;
	new->tex_ea.reflectance = 0;
	new->tex_fl.reflectance = 0;
	new->tex_no.reflectance = 0;
	new->tex_so.reflectance = 0;
	new->tex_we.reflectance = 0;
	new->ceil_height = -1;
	new->floor_height = -1;
	new->is_wall = -1;
	return (new);
}

void	retrieve_tile(t_tile **tiles, int map_fd, char *line, int *err)
{
	char	*temp;

	if (*err)
		return ;
	if (!*tiles)
		*tiles = new_tile();
	if (!*tiles)
		return (ft_perror(-1, "Internal error: malloc.", 0), *err = 1, VOID);
	while (1)
	{
		temp = get_next_line(map_fd);
		if (!temp)
			return (ft_perror(-1, "Internal error: malloc.", 0), *err = 1, VOID);
		line = ft_strtrim(temp, "\t\n ");
		ft_del((void **)&temp);
		if (!line)
			return (ft_perror(-1, "Internal error: malloc.", 0), *err = 1, VOID);
		if (ft_strncmp("}", line, 2) == 0)
			return (ft_del((void **)&line), add_wpath(*tiles, err), VOID);
		retrieve_switch(*tiles, line, err);
		ft_del((void **)&line);
		if (*err)
			return ;
	}
}
