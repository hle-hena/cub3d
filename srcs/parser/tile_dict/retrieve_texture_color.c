/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   retrieve_texture_color.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 12:49:56 by hle-hena          #+#    #+#             */
/*   Updated: 2025/08/29 14:18:26 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	skip_pattern(char **arg, char *pattern)
{
	int	i;

	i = 0;
	while (pattern[i])
	{
		if (pattern[i] != **arg)
		{
			if (!**arg)
				return (1);
			if (pattern[i] == ' ')
			{
				i++;
				continue ;
			}
			return (1);
		}
		if (pattern[i] != ' ')
			i++;
		(*arg)++;
	}
	return (0);
}

int	ft_atoi_err(char **arg)
{
	int	nb;
	int	digits;

	nb = 0;
	digits = 0;
	if (!(*arg))
		return (-1);
	while (ft_isspace(**arg))
		(*arg)++;
	if (!**arg || !ft_isdigit(**arg))
		return (-1);
	while (**arg == '0' && ft_isdigit(*((*arg) + 1)))
		(*arg)++;
	while (ft_isdigit(**arg))
	{
		nb = nb * 10 + (**arg - '0');
		(*arg)++;
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
		return (*err = 1, ft_perror(-1, "Internal error: malloc.", 0));
	(*img)->img = mlx_new_image(get_data()->mlx, 1, 1);
	if (!(*img)->img)
		return (*err = 1, ft_perror(-1, ft_strsjoin((char *[]){"An error \
happened during the opening of the image for '", line, "'.", NULL}), 1));
	(*img)->height = 1;
	(*img)->width = 1;
	(*img)->data = mlx_get_data_addr((*img)->img, &(*img)->bpp,
			&(*img)->size_line, &(*img)->endian);
	(*img)->bpp /= 8;
	(*img)->size_line /= (*img)->bpp;
	(*img)->endian = 0;
	*(int *)(*img)->data = ((color.re << 16) | (color.gr << 8) | color.bl);
}

int	retrieve_color(char **line, int *color, int *err, char *color_name)
{
	*color = ft_atoi_err(line);
	if (*err == -1)
	{
		ft_perror(-1, ft_strsjoin((char *[]){"Expected a positive number under\
 255 as ", color_name, ".", NULL}), 0);
		*err = 1;
		return (1);
	}
	return (0);
}

void	retrieve_texture_color(t_img **img, char *line, int *err)
{
	t_col	color;

	if (ft_strnstr(line, ".xpm", ft_strlen(line)))
		return (*err = 1, ft_perror(-1, "Assets should be in a subfolder.", 0));
	if (retrieve_color(&line, &color.re, err, "red"))
		return ;
	if (skip_pattern(&line, " , "))
		return (*err = 1, ft_perror(-1, "Wrong pattern recognised for an rgb \
color.", 0));
	if (retrieve_color(&line, &color.gr, err, "green"))
		return ;
	if (skip_pattern(&line, " , "))
		return (*err = 1, ft_perror(-1, "Wrong pattern recognised for an rgb \
color.", 0));
	if (retrieve_color(&line, &color.bl, err, "blue"))
		return ;
	if (*line)
		return (*err = 1, ft_perror(-1, "Expected a \\n after the last color.",
				0));
	set_img_color(img, line, color, err);
}
