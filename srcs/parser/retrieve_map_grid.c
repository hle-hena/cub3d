/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   retrieve_map_grid.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 16:37:49 by hle-hena          #+#    #+#             */
/*   Updated: 2025/04/13 11:31:26 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	resize_lines(t_list *grid, size_t max_wid, int *err)
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
			return (*err = 1, (void)0);
		i = ft_strlcpy(temp, grid->content, max_wid + 1);
		ft_memset(temp + i, '1', max_wid - i);
		temp[max_wid] = 0;
		ft_del((void **)&grid->content);
		grid->content = temp;
	}
}

int	check_line(t_map *map, char *line)
{
	int	len;

	len = -1;
	while (line[++len])
	{
		if (ft_strchr("01", line[len]))
			continue ;
		else if (ft_strchr("NSWE", line[len]))
		{
			if (map->player.y != -1)
				return (ft_perror(-1, "Too many players.", 0), -1);
			map->player.x = len;
			map->player.y = map->len;
			map->player.rot = 180 * (line[len] == 'W') + 90 * (line[len] == 'S')
				+ 270 * (line[len] == 'N');
		}
		else if (line[len] == '\n')
			return (len);
		else if (line[len] == ' ')
			line[len] = '1';
		else
			return (ft_perror(-1, ft_strsjoin((char *[]){"Invalid line in map :\
 '", line, "'.", NULL}), 1), -1);
	}
	return (len);
}

void	set_map(t_map *map, t_list *grid, int *err)
{
	int		len;
	t_list	*next;

	if (!grid)
		return (ft_perror(-1, "Missing map.", 0), *err = 1, (void)0);
	len = 0;
	resize_lines(grid, map->wid, err);
	if (*err)
		return (ft_perror(-1, "cub3d: Internal error: malloc", 0),
			ft_lstclear(&grid, ft_del), *err = 1, (void)0);
	map->matrix = malloc((map->len + 1) * sizeof(char *));
	if (!map->matrix)
		return (ft_perror(-1, "cub3d: Internal error: malloc", 0),
			ft_lstclear(&grid, ft_del), *err = 1, (void)0);
	while (grid)
	{
		map->matrix[len] = grid->content;
		len++;
		next = grid->next;
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
		if (!line)
			break ;
		if (ft_strncmp(line, "\n", 2) != 0)
			break ;
		ft_del((void **)&line);
	}
	if (line && grid)
		return (ft_perror(-1, "Empty line in map.", 0), ft_del((void **)&line),
			ft_lstclear(&grid, ft_del), *err = 1, NULL);
	return (line);
}

void	retrieve_map_grid(t_map *map, int map_fd, int *err)
{
	t_list	*grid;
	char	*line;
	int		wid;

	grid = NULL;
	line = handle_empty_line(grid, map_fd, err);
	while (1)
	{
		if (!line)
			break ;
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
			return (ft_lstclear(&grid, ft_del), *err = 1, (void)0);
		line[wid] = 0;
		add_link(&grid, line);
		map->len++;
		if (wid > map->wid)
			map->wid = wid;
		line = get_next_line(map_fd);
	}
	set_map(map, grid, err);
}
