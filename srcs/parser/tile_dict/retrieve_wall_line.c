/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   retrieve_wall_line.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 10:52:00 by hle-hena          #+#    #+#             */
/*   Updated: 2025/08/07 16:09:52 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static inline int	retrieve_coo(char **arg, int min, int max, t_vec *coo)
{
	coo->x = ft_atof_err(*arg, min, max, arg);
	if (coo->x == -1)
		return (ft_perror(-1, "Expecting a positive number between 0 and 1 as \
coordinate x of a path.", 0), 1);
	if (skip_pattern(arg, " , "))
		return (ft_perror(-1, "Wrong pattern recognised during the parsing of a\
 set of coordinate. Should be '(x, y)", 0), 1);
	if (!**arg)
		return (ft_perror(-1, "Unexpected end of line while parsing a \
coordinate for a path.", 0), 1);
	coo->y = ft_atof_err(*arg, min, max, arg);
	if (coo->y == -1)
		return (ft_perror(-1, "Expecting a positive number between 0 and 1 as \
coordinate y of a path.", 0), 1);
	return (0);
}

static inline int	retrieve_reflectance(char **arg, float *reflectance)
{
	if (!**arg)
		return (0);
	if (skip_pattern(arg, " : "))
	{
		ft_perror(-1, "Wrong pattern for the reflectance. Should be ': [value]'\
", 0);
		return (1);
	}
	*reflectance = ft_atof_err(*arg, 0, 1, arg);
	if (*reflectance == -1)
	{
		ft_perror(-1, "Expecting a positive value between 0 and 1 for \
reflectance.", 0);
		return (1);
	}
	return (0);
}

void	retrieve_line(t_tile *tile, char *arg, int *err)
{
	t_wpath	*line;

	line = malloc(sizeof(t_wpath));
	if (!line)
		return (ft_del((void **)&line), *err = 1, VOID);
	if (skip_pattern(&arg, "( ( "))
		return (ft_del((void **)&line), *err = 1, VOID);
	if (retrieve_coo(&arg, 0, 1, &line->start))
		return (ft_del((void **)&line), *err = 1, VOID);
	if (skip_pattern(&arg, " ) , ( "))
		return (ft_del((void **)&line), *err = 1, VOID);
	if (retrieve_coo(&arg, 0, 1, &line->end))
		return (ft_del((void **)&line), *err = 1, VOID);
	if (skip_pattern(&arg, " ) )"))
		return (ft_del((void **)&line), *err = 1, VOID);
	line->texture.img = NULL;
	line->normal = vec_normalize((t_vec){
			-line->end.y + line->start.y,
			line->end.x - line->start.x});
	line->reflectance = 0;
	line->mode = 0;
	line->center = (t_vec){0};
	if (retrieve_reflectance(&arg, &line->reflectance))
		return (ft_del((void **)&line), *err = 1, VOID);
	add_link(&tile->wpath, line);
}

int	retrieve_arc_coo(t_wpath *arc, char **arg, int *err)
{
	if (skip_pattern(arg, "( ( "))
		return (ft_del((void **)&arc), *err = 1, 1);
	if (retrieve_coo(arg, 0, 1, &arc->start))
		return (ft_del((void **)&arc), *err = 1, 1);
	if (skip_pattern(arg, " ) , ( "))
		return (ft_del((void **)&arc), *err = 1, 1);
	if (retrieve_coo(arg, 0, 1, &arc->end))
		return (ft_del((void **)&arc), *err = 1, 1);
	if (skip_pattern(arg, " ) , ( "))
		return (ft_del((void **)&arc), *err = 1, 1);
	if (retrieve_coo(arg, 0, 1, &arc->center))
		return (ft_del((void **)&arc), *err = 1, 1);
	if (skip_pattern(arg, " ) )"))
		return (ft_del((void **)&arc), *err = 1, 1);
	return (0);
}

void	retrieve_arc(t_tile *tile, char *arg, int *err)
{
	t_wpath	*arc;
	float	r1;
	float	r2;

	arc = malloc(sizeof(t_wpath));
	if (!arc)
		return (ft_del((void **)&arc), *err = 1, VOID);
	if (retrieve_arc_coo(arc, &arg, err))
		return ;
	arc->texture.img = NULL;
	arc->reflectance = 0;
	arc->mode = 1;
	r1 = sqrtf(vec_len2(vec_sub(arc->start, arc->center)));
	r2 = sqrtf(vec_len2(vec_sub(arc->end, arc->center)));
	if (fabsf(r1 - r2) > 1e-4f)
	{
		ft_perror(-1, "The second point of the arc should be at the same \
distance to the center as the first.", 0);
		return (ft_del((void **)&arc), *err = 1, VOID);
	}
	if (retrieve_reflectance(&arg, &arc->reflectance))
		return (ft_del((void **)&arc), *err = 1, VOID);
	add_link(&tile->wpath, arc);
}
