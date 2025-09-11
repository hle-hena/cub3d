/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   retrieve_map.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 18:02:27 by hle-hena          #+#    #+#             */
/*   Updated: 2025/09/11 14:50:31 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D_bonus.h"

void	resize_lines(t_list *grid, char void_char, size_t max_wid, int *err)
{
	char	*temp;
	int		i;

	while (grid)
	{
		if (ft_strlen(grid->content) == max_wid)
		{
			grid = grid->next;
			continue ;
		}
		temp = malloc((max_wid + 1) * sizeof(char));
		if (!temp)
			return (*err = 1, ft_perror(-1, "Internal error: malloc", 0));
		i = ft_strlcpy(temp, grid->content, max_wid + 1);
		ft_memset(temp + i, void_char, max_wid - i);
		temp[max_wid] = 0;
		ft_del((void **)&grid->content);
		grid->content = temp;
	}
}

void	set_map(t_map *map, t_list *grid, int *err)
{
	int		len;
	int		i;
	t_list	*next;

	if (!grid)
		return (*err = 1, ft_perror(-1, "Missing map.", 0));
	len = 0;
	resize_lines(grid, map->void_char, map->wid, err);
	if (*err)
		return (ft_lstclear(&grid, ft_del));
	map->map = malloc((map->len * map->wid) * sizeof(int));
	if (!map->map)
		return (ft_perror(-1, "cub3D: Internal error: malloc", 0),
			*err = 1, ft_lstclear(&grid, ft_del));
	while (grid)
	{
		i = -1;
		while (++i < map->wid)
			*(map->map + len * map->wid + i) = ((char *)grid->content)[i];
		len++;
		next = grid->next;
		ft_del((void **)&grid->content);
		ft_del((void **)&grid);
		grid = next;
	}
}

char	*handle_empty_line(t_list *grid, int map_fd, int *err)
{
	char	*line;

	while (1)
	{
		line = get_next_line(map_fd);
		if (!line || ft_strncmp(line, "\n", 2) != 0)
			break ;
		ft_del((void **)&line);
	}
	if (line)
		return (ft_perror(-1, "Empty line in map.", 0), ft_del((void **)&line),
			ft_lstclear(&grid, ft_del), *err = 1, NULL);
	return (line);
}

int	check_line(t_map *map, char *line)
{
	int	len;

	len = -1;
	while (line[++len])
	{
		if (get_tile_dict()[(int)line[len]])
			continue ;
		else if (ft_strchr("NSWE", line[len]))
		{
			if (map->player.y != -1)
				return (ft_perror(-1, "Too many players.", 0), -1);
			map->player.x = 0.5f + len;
			map->player.y = 0.5f + map->len;
			map->player.rot = 180 * (line[len] == 'W') + 90 * (line[len] == 'S')
				+ 270 * (line[len] == 'N');
			line[len] = map->replace_tile;
		}
		else if (line[len] == '\n')
			return (len);
		else if (line[len] != map->void_char)
			return (ft_perror(-1, ft_strsjoin((char *[]){"Invalid line in map :\
 '", line, "'. A char is not in the tile dir.", NULL}), 1), -1);
	}
	return (len);
}

void	retrieve_map(t_map *map, char *line, int map_fd, int *err)
{
	t_list	*grid;
	int		wid;

	grid = NULL;
	while (line)
	{
		if (ft_strncmp(line, "\n", 2) == 0)
		{
			ft_del((void **)&line);
			handle_empty_line(grid, map_fd, err);
			if (*err)
				return ;
			break ;
		}
		wid = check_line(map, line);
		if (wid == -1)
			return (ft_lstclear(&grid, ft_del), *err = 1,
				ft_del((void **)&line));
		line[wid] = 0;
		add_link(&grid, line);
		map->len++;
		map->wid = ft_max(wid, map->wid);
		line = get_next_line(map_fd);
	}
	set_map(map, grid, err);
}
