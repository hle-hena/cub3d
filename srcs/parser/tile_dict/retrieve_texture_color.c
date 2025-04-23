/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   retrieve_texture_color.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 12:49:56 by hle-hena          #+#    #+#             */
/*   Updated: 2025/04/23 13:47:57 by hle-hena         ###   ########.fr       */
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

void	set_img_color(t_img **img, char *line, t_col color, int *err)
{
	*img = malloc(sizeof(t_img));
	if (!*img)
		return (ft_perror(-1, "Internal error: malloc.", 0), *err = 1, VOID);
	(*img)->img = mlx_new_image(get_data()->mlx, 1, 1);
	if (!(*img)->img)
		return (ft_perror(-1, ft_strsjoin((char *[]){"An error happened during \
the opening of the image for '", line, "'.", NULL}), 1), *err = 1, VOID);
	(*img)->height = 1;
	(*img)->width = 1;
	(*img)->data = mlx_get_data_addr((*img)->img, &(*img)->bpp, &(*img)->size_line,
		&(*img)->endian);
	(*img)->bpp /= 8;
	(*img)->endian = 0;
	*(int *)(*img)->data = calc_color(color);
}

void	retrieve_texture_color(t_img **img, char *line, int *err)
{
	t_col	color;
	int		i;

	i = 0;
	if (ft_strnstr(line, ".xpm", ft_strlen(line)))
		return (ft_perror(-1, "Assets should be in a subfolder.", 0), *err = 1,
			VOID);
	color.re = ft_atoi_err(line, &i);
	if (color.re == -1 || line[i] != ',')
		return (ft_perror(-1, ft_strsjoin((char *[]){"Expected a positive numbe\
r under 255 as color 1. Instead got ", line, ".", NULL}), 1), *err = 1, VOID);
	i++;
	color.gr = ft_atoi_err(&line[i], &i);
	if (color.gr == -1 || line[i] != ',')
		return (ft_perror(-1, ft_strsjoin((char *[]){"Expected a positive numbe\
r under 255 as color 2. Instead got ", line, ".", NULL}), 1), *err = 1, VOID);
	i++;
	color.bl = ft_atoi_err(&line[i], &i);
	if (color.bl == -1 || line[i])
		return (ft_perror(-1, ft_strsjoin((char *[]){"Expected a positive numbe\
r under 255 as color 3. Instead got ", line, ".", NULL}), 1), *err = 1, VOID);
	set_img_color(img, line, color, err);
}
