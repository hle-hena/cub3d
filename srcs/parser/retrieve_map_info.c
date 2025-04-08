/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   retrieve_map_info.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 16:37:27 by hle-hena          #+#    #+#             */
/*   Updated: 2025/04/08 17:20:32 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	safe_color_add(t_map *map, char *line, int *err)
{
	if (ft_strncmp(line, "F ", 2) == 0)
	{
		if (map->floor.bl != -1)
			return (ft_perror(-1, "Duplicate of floor.", 0), *err = 1, 1);
		return (map->floor = get_color(ft_strtrim(line + 2, " \n"), err), 1);
	}
	else if (ft_strncmp(line, "C ", 2) == 0)
	{
		if (map->ceiling.bl != -1)
			return (ft_perror(-1, "Duplicate of ceiling.", 0), *err = 1, 1);
		return (map->ceiling = get_color(ft_strtrim(line + 2, " \n"), err), 1);
	}
	return (0);
}

int	safe_texture_add(t_map *map, char *line, int *err)
{
	if (ft_strncmp(line, "NO ", 3) == 0)
	{
		if (map->img_no.img)
			return (ft_perror(-1, "Duplicate of texture NO.", 0), *err = 1, 1);
		return (map->img_no = get_path(ft_strtrim(line + 3, " \n"), err), 1);
	}
	else if (ft_strncmp(line, "SO ", 3) == 0)
	{
		if (map->img_so.img)
			return (ft_perror(-1, "Duplicate of texture SO.", 0), *err = 1, 1);
		return (map->img_so = get_path(ft_strtrim(line + 3, " \n"), err), 1);
	}
	else if (ft_strncmp(line, "WE ", 3) == 0)
	{
		if (map->img_we.img)
			return (ft_perror(-1, "Duplicate of texture WE.", 0), *err = 1, 1);
		return (map->img_we = get_path(ft_strtrim(line + 3, " \n"), err), 1);
	}
	else if (ft_strncmp(line, "EA ", 3) == 0)
	{
		if (map->img_ea.img)
			return (ft_perror(-1, "Duplicate of texture EA.", 0), *err = 1, 1);
		return (map->img_ea = get_path(ft_strtrim(line + 3, " \n"), err), 1);
	}
	return (0);
}

void	retrieve_info(t_map *map, char *line, int *err)
{
	if (safe_texture_add(map, line, err))
		return ;
	else if (safe_color_add(map, line, err))
		return ;
	else
		(print_missing_info(map), *err = 1);
}

void	retrieve_map_info(t_map *map, int map_fd, int *err)
{
	char	*temp;
	char	*line;

	while (1)
	{
		temp = get_next_line(map_fd);
		if (!temp)
			break ;
		if (ft_strncmp(temp, "\n", 2) == 0)
		{
			ft_del((void **)&temp);
			continue ;
		}
		line = ft_strtrim(temp, " \t\n");
		ft_del((void **)&temp);
		retrieve_info(map, line, err);
		ft_del((void **)&line);
		if (*err)
			return ;
		if (has_all_info(map))
			return ;
	}
	if (!has_all_info(map))
		(print_missing_info(map), *err = 1);
}
