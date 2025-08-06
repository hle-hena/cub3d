/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   retrieve_light.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 10:55:35 by hle-hena          #+#    #+#             */
/*   Updated: 2025/08/06 14:17:45 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

t_vec	retrieve_light_pos(char **line, int *err)
{
	t_vec	pos;

	(*line)++;
	pos.x = ft_atof_err(*line, 0, -1, line);
	if (pos.x == -1 || **line != ',')
		return (ft_perror(-1, ft_strsjoin((char *[]){"Unexpected char near \
'", *line, "'. Expected a ','.", NULL}), 1), *err = 1, (t_vec){0});
	(*line)++;
	pos.y = ft_atof_err(*line, 0, -1, line);
	if (pos.y == -1 || **line != ')')
		return (ft_perror(-1, ft_strsjoin((char *[]){"Unexpected char near \
'", *line, "'. Expected a ')'.", NULL}), 1), *err = 1, (t_vec){0});
	(*line)++;
	return (pos);
}

t_col	retrieve_light_color(char **line, int *err)
{
	t_col	color;

	(*line)++;
	color.re = ft_atoi_err(line);
	if (color.re == -1)
		return (ft_perror(-1, "Expected a positive number under 255 as red.\
", 0), *err = 1, (t_col){0});
	if (skip_pattern(line, " , "))
		return (ft_perror(-1, "Wrong pattern recognised for an rgb color.", 0),
			*err = 1, (t_col){0});
	color.gr = ft_atoi_err(line);
	if (color.gr == -1)
		return (ft_perror(-1, "Expected a positive number under 255 as green.\
", 0), *err = 1, (t_col){0});
	if (skip_pattern(line, " , "))
		return (ft_perror(-1, "Wrong pattern recognised for an rgb color.", 0),
			*err = 1, (t_col){0});
	color.bl = ft_atoi_err(line);
	if (color.bl == -1)
		return (ft_perror(-1, "Expected a positive number under 255 as bl.\
", 0), *err = 1, (t_col){0});
	if (skip_pattern(line, " } "))
		return (ft_perror(-1, "Wrong pattern recognised for an rgb color.", 0),
			*err = 1, (t_col){0});
	return (color);
}

int	finished(char **line, int *err)
{
	if (**line != ')' && **line != ',')
		(ft_perror(0, ft_strsjoin((char *[]){"The separator in the light is \
wrong. expected ',' or ')'. Got '", *line, "'.", NULL}), 1), *err = 1);
	return (**line != ')');
}

int	retrieve_light_info(t_lmap *lmap, char **line, int *err)
{
	if (**line == '(')
	{
		if (lmap->lights[lmap->nb_ls].pos.y != -1)
			return (ft_perror(-1, "Duplicated coordinates in a light \
definition.", 0), *err = 1, 0);
		lmap->lights[lmap->nb_ls].pos = retrieve_light_pos(line, err);
	}
	else if (**line == '{')
	{
		if (lmap->lights[lmap->nb_ls].color.re != -1)
			return (ft_perror(-1, "Duplicated color in a light \
definition.", 0), *err = 1, 0);
		lmap->lights[lmap->nb_ls].color = retrieve_light_color(line, err);
	}
	else
	{
		if (lmap->lights[lmap->nb_ls].emittance != -1)
			return (ft_perror(-1, "Duplicated emittance in a light \
definition.", 0), *err = 1, 0);
		lmap->lights[lmap->nb_ls].emittance = ft_atof_err(*line, 0, 1, line);
		if (lmap->lights[lmap->nb_ls].emittance == -1)
			return (ft_perror(-1, "The value given for the emittance is not \
correct.", 0), *err = 1, 0);
	}
	return (finished(line, err));
}

void	retrieve_light(char *line, int *err)
{
	t_lmap	*lmap;

	lmap = &get_data()->lmap;
	lmap->lights = ft_realloc(lmap->lights, (lmap->nb_ls + 1) * sizeof(t_light),
			lmap->nb_ls * sizeof(t_light));
	lmap->lights[lmap->nb_ls].pos.y = -1;
	lmap->lights[lmap->nb_ls].emittance = -1;
	lmap->lights[lmap->nb_ls].color = (t_col){-1, -1, -1};
	while (retrieve_light_info(lmap, &line, err))
	{
		if (*err)
			return ;
		line++;
		while (ft_isspace(*line))
			line++;
	}
	if (lmap->lights[lmap->nb_ls].pos.y == -1)
		return (ft_perror(0, "A light is missing its coordinates.", 0),
			*err = 1, VOID);
	if (lmap->lights[lmap->nb_ls].emittance < 0)
		lmap->lights[lmap->nb_ls].emittance = 1;
	if (lmap->lights[lmap->nb_ls].color.bl < 0)
		lmap->lights[lmap->nb_ls].color = (t_col){255, 255, 255};
	lmap->nb_ls++;
	return ;
}
