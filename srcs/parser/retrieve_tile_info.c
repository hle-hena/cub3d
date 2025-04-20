/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   retrieve_tile_info.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 14:28:34 by hle-hena          #+#    #+#             */
/*   Updated: 2025/04/20 21:54:25 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	type_switch(t_tile *tile, char *line, int *err)
{
	char	*arg;

	if (!line[0] || line[0] == '#')
		return ;
	arg = ft_strtrim(line + 2, " \t\n"); //This is unsafe, what if line is < 2
	if (!arg)
		return (*err = 1, VOID);
	if (!*arg)
		return (ft_perror(-1, ft_strsjoin((char *[]){"Empty line after \
identifier at ", line, ".", NULL}), 1), ft_del((void **)&arg), *err = 1, VOID);
	if (ft_strncmp("NO ", line, 3) == 0)
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
	else
		return (ft_perror(-1, ft_strsjoin((char *[]){"Invalid identifier at \
line : ", line, ".", NULL}), 1), ft_del((void **)&arg), *err = 1, VOID);
	ft_del((void **)&arg);
}

t_tile	*new_tile(void)
{
	t_tile	*new;

	new = malloc(sizeof(t_tile));
	if (!new)
		return (NULL);
	new->tex_ce.img = NULL;
	new->tex_ea.img = NULL;
	new->tex_fl.img = NULL;
	new->tex_no.img = NULL;
	new->tex_so.img = NULL;
	new->tex_we.img = NULL;
	new->ceil_height = -1;
	new->floor_height = -1;
	new->is_wall = -1;
	return (new);
}

void	retrieve_tile(t_map *map, int map_fd, char *line, int *err)
{
	char	*temp;
	t_tile	**tile;

	tile = &map->tiles[(int)(*line)];
	if (!*tile)
		*tile = new_tile();
	if (!*tile)
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
			return (ft_del((void **)&line), VOID);
		type_switch(*tile, line, err);
		ft_del((void **)&line);
		if (*err)
			return ;
	}
}
