/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   retrieve_wall_line.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 10:52:00 by hle-hena          #+#    #+#             */
/*   Updated: 2025/07/28 16:10:36 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static inline int	skip_pattern(char **arg, char *pattern)
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

static inline int	retrieve_coo(char **arg, int min, int max, t_vec *coo)
{
	coo->x = ft_atof_err(*arg, min, max, arg);
	if (coo->x == -1)
		return (1);
	if (skip_pattern(arg, " , "))
		return (1);
	if (!**arg)
		return (1);
	coo->y = ft_atof_err(*arg, min, max, arg);
	if (coo->y == -1)
		return (1);
	return (0);
}

static inline int	retrieve_reflectance(char **arg, float *reflectance)
{
	if (!**arg)
		return (0);
	if (skip_pattern(arg, " , "))
		return (1);
	*reflectance = ft_atof_err(*arg, 0, 1, arg);
	if (*reflectance == -1)
		return (1);
	return (0);
}

void	retrieve_line(t_tile *tile, char *arg, int *err)
{
	t_wpath	*line;

	line = malloc(sizeof(t_wpath));
	if (!line)
		return (*err = 1, VOID);
	if (skip_pattern(&arg, "( ( "))
		return (*err = 1, VOID);
	if (retrieve_coo(&arg, 0, 1, &line->start))
		return (*err = 1, VOID);
	if (skip_pattern(&arg, " ) , ( "))
		return (*err = 1, VOID);
	if (retrieve_coo(&arg, 0, 1, &line->end))
		return (*err = 1, VOID);
	if (skip_pattern(&arg, " ) )"))
		return (*err = 1, VOID);
	line->texture.img = NULL;
	line->normal = normalize((t_vec){
			-line->end.y + line->start.y,
			line->end.x - line->start.x});
	line->reflectance = 0;
	line->mode = 0;
	line->center = (t_vec){0};
	if (retrieve_reflectance(&arg, &line->reflectance))
		return (*err = 1, VOID);
	add_link(&tile->wpath, line);
}

void	retrieve_arc(t_tile *tile, char *arg, int *err)
{
	t_wpath	*line;

	line = malloc(sizeof(t_wpath));
	if (!line)
		return (*err = 1, VOID);
	if (skip_pattern(&arg, "( ( "))
		return (*err = 1, VOID);
	if (retrieve_coo(&arg, 0, 1, &line->start))
		return (*err = 1, VOID);
	if (skip_pattern(&arg, " ) , ( "))
		return (*err = 1, VOID);
	if (retrieve_coo(&arg, 0, 1, &line->end))
		return (*err = 1, VOID);
	if (skip_pattern(&arg, " ) , ( "))
		return (*err = 1, VOID);
	if (retrieve_coo(&arg, 0, 1, &line->center))
		return (*err = 1, VOID);
	if (skip_pattern(&arg, " ) )"))
		return (*err = 1, VOID);
	line->texture.img = NULL;
	line->normal = normalize((t_vec){
			-line->end.y + line->start.y,
			line->end.x - line->start.x});
	line->reflectance = 0;
	line->mode = 1;
	if (retrieve_reflectance(&arg, &line->reflectance))
		return (*err = 1, VOID);
	add_link(&tile->wpath, line);
}
