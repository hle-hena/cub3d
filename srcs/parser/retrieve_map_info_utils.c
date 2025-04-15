/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   retrieve_map_info_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 16:26:41 by hle-hena          #+#    #+#             */
/*   Updated: 2025/04/15 12:17:15 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	ft_atoi_err(char *arg, int *index)
{
	int	nb;
	int	digits;

	nb = 0;
	digits = 0;
	if (!arg)
		return (-1);
	while (ft_isspace(*arg))
		(arg++, ((*index)++));
	if (!*arg || !ft_isdigit(*arg))
		return (-1);
	while (*arg == '0' && ft_isdigit(*(arg + 1)))
		(arg++, ((*index)++));
	while (ft_isdigit(*arg))
	{
		nb = nb * 10 + (*arg - '0');
		(*index)++;
		arg++;
		digits++;
		if (nb > 255)
			return (-1);
	}
	return (nb);
}

int	has_all_info(t_map *map)
{
	if (!map->img_ea.img || !map->img_we.img || !map->img_so.img
		|| !map->img_no.img)
		return (0);
	if (map->ceiling.bl == -1 || map->floor.bl == -1)
		return (0);
	return (1);
}

void	print_missing_info(t_map *map)
{
	if (!map->img_ea.img)
		ft_perror(-1, "Missing file for east wall.", 0);
	if (!map->img_we.img)
		ft_perror(-1, "Missing file for west wall.", 0);
	if (!map->img_no.img)
		ft_perror(-1, "Missing file for north wall.", 0);
	if (!map->img_so.img)
		ft_perror(-1, "Missing file for south wall.", 0);
	if (map->ceiling.bl == -1)
		ft_perror(-1, "Missing color for the ceiling.", 0);
	if (map->floor.bl == -1)
		ft_perror(-1, "Missing color for the floor.", 0);
}

t_col	get_color(char *line, int *err)
{
	t_col	color;
	int		i;

	if (!line)
		return (*err = 1, ft_perror(-1, "The color was invalid, or the malloc f\
ailed.", 0), (t_col){0});
	i = 0;
	color.re = ft_atoi_err(line, &i);
	if (color.re == -1 || line[i] != ',')
		return (*err = 1, ft_perror(-1, "Invalid color given as color number 1.\
", 0), ft_del((void **)&line), (t_col){0});
	i++;
	color.gr = ft_atoi_err(&line[i], &i);
	if (color.gr == -1 || line[i] != ',')
		return (*err = 1, ft_perror(-1, "Invalid color given as color number 2.\
", 0), ft_del((void **)&line), (t_col){0});
	i++;
	color.bl = ft_atoi_err(&line[i], &i);
	if (color.bl == -1 || line[i])
		return (*err = 1, ft_perror(-1, "Invalid color given as color number 3.\
", 0), ft_del((void **)&line), (t_col){0});
	ft_del((void **)&line);
	return (color);
}

t_img	get_path(char *line, int *err)
{
	char	*extension;
	t_img	img;

	if (!line)
		return (*err = 1, ft_perror(-1, "The name of the map was invalid, or th\
e malloc failed.", 0), (t_img){0});
	extension = ft_strrchr(line, '.');
	if (!extension)
		return (*err = 1, ft_perror(-1, "Invalid usage. Assets should be in .xp\
m", 0), ft_del((void **)&line), (t_img){0});
	if (ft_strncmp(".xpm", extension, 5))
		return (*err = 1, ft_perror(-1, "Invalid usage. Assets should be in .xp\
m", 0), ft_del((void **)&line), (t_img){0});
	img.img = mlx_xpm_file_to_image(get_data()->mlx, line,
			&img.width, &img.height);
	if (!img.img)
	{
		extension = ft_strsjoin((char *[]){"An error happened during the openin\
g of the file '", line, "'.", NULL});
		ft_del((void **)&line);
		return (*err = 1, ft_perror(-1, extension, 1), (t_img){0});
	}
	ft_del((void **)&line);
	img.data = mlx_get_data_addr(img.img, &img.bpp, &img.size_line, &img.endian);
	return (img.bpp /= 8, img);
}
