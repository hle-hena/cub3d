/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   retrieve_map_info.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 16:37:27 by hle-hena          #+#    #+#             */
/*   Updated: 2025/04/07 16:31:30 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

t_col	get_color(char *line, int *err)
{
	t_col	color;
	int		i;

	if (!line)
		return (*err = 1, ft_putstr_fd("The color was invalid, or the malloc fa\
iled.", 2), (t_col){0});
	i = 0;
	color.re = ft_atoi_err(line, &i);
	if (color.re == -1 || line[i] != ',')
		return (*err = 1, ft_putstr_fd("Invalid color given as color number 1.\
", 2), ft_del((void **)&line), (t_col){0});
	i++;
	color.gr = ft_atoi_err(&line[i], &i);
	if (color.gr == -1 || line[i] != ',')
		return (*err = 1, ft_putstr_fd("Invalid color given as color number 2.\
", 2), ft_del((void **)&line), (t_col){0});
	i++;
	color.bl = ft_atoi_err(&line[i], &i);
	if (color.bl == -1 || line[i])
		return (*err = 1, ft_putstr_fd("Invalid color given as color number 3.\
", 2), ft_del((void **)&line), (t_col){0});
	ft_del((void **)&line);
	return (color);
}

t_img	get_path(char *line, int *err)
{
	char	*extension;
	t_img	img;

	if (!line)
		return (*err = 1, ft_putstr_fd("The name of the map was invalid, or the\
 malloc failed.", 2), (t_img){0});
	extension = ft_strrchr(line, '.');
	if (!extension)
		return (*err = 1, ft_putstr_fd("Invalid usage. Assets should be in .xpm\
", 2), ft_del((void **)&line), (t_img){0});
	if (ft_strncmp(".xpm", extension, 5))
		return (*err = 1, ft_putstr_fd("Invalid usage. Assets should be in .xpm\
", 2), ft_del((void **)&line), (t_img){0});
	img.img = mlx_xpm_file_to_image(get_data()->mlx, line,
			&img.width, &img.height);
	if (!img.img)
		return (*err = 1, ft_perror(-1, ft_strsjoin((char *[]){"An error happen\
ed during the opening of the file '", line, "'.", NULL}), 0), (t_img){0});
	return (img);
}

void	retrieve_info(t_map **map, char *line, int *err)
{
	if (ft_strncmp(line, "NO ", 3) == 0)
		(*map)->img_no = get_path(ft_strtrim(line + 3, " \n"), err);
	else if (ft_strncmp(line, "SO ", 3) == 0)
		(*map)->img_so = get_path(ft_strtrim(line + 3, " \n"), err);
	else if (ft_strncmp(line, "EA ", 3) == 0)
		(*map)->img_ea = get_path(ft_strtrim(line + 3, " \n"), err);
	else if (ft_strncmp(line, "WE ", 3) == 0)
		(*map)->img_we = get_path(ft_strtrim(line + 3, " \n"), err);
	else if (ft_strncmp(line, "F ", 2) == 0)
		(*map)->floor = get_color(ft_strtrim(line + 2, " \n"), err);
	else if (ft_strncmp(line, "C ", 2) == 0)
		(*map)->floor = get_color(ft_strtrim(line + 2, " \n"), err);
	else
		*err = 1;
}

void	retrieve_map_info(t_map **map, char *map_name, int *err)
{
	char	*temp;
	char	*line;

	while (1)
	{
		temp = get_next_line(0);
		if (temp)
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
		*err = 1;
}
