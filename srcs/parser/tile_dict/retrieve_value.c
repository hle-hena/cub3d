/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   retrieve_value.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 16:34:13 by hle-hena          #+#    #+#             */
/*   Updated: 2025/04/24 15:09:57 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	get_limits(int *limits, char **lim_str, char *id)
{
	limits[0] = 0;
	limits[1] = 0;
	lim_str[0] = "0";
	lim_str[1] = "0";
	if (ft_strncmp("CH", id, 3) == 0)
	{
		limits[0] = 1;
		limits[1] = 100;
		lim_str[0] = "1";
		lim_str[1] = "100";
	}
	else if (ft_strncmp("FH", id, 3) == 0)
	{
		limits[0] = 0;
		limits[1] = 99;
		lim_str[0] = "0";
		lim_str[1] = "99";
	}
	else if (ft_strncmp("W", id, 3) == 0)
	{
		limits[0] = 0;
		limits[1] = 1;
		lim_str[0] = "0";
		lim_str[1] = "1";
	}
}

void	retrieve_value(int *value, char *arg, int *err, char *id)
{
	int		limits[2];
	char	*lim_str[2];

	if (*value != -1)
		return (ft_perror(-1, ft_strsjoin((char *[]){"Duplicate of identifier '\
", id, "'.", NULL}), 1), *err = 1, VOID);
	(get_limits(limits, lim_str, id), *value = 0);
	while (*arg == '0' && ft_isdigit(*(arg + 1)))
		arg++;
	while (ft_isdigit(*arg))
	{
		*value = (*value) * 10 + (*arg - '0');
		arg++;
		if (*value > limits[1])
			return (ft_perror(-1, ft_strsjoin((char *[]){"Expected a positive \
number smaller than ", lim_str[1], " for identifier '", id ,"'.", NULL}), 1),
				*err = 1, VOID);
	}
	if (*arg)
		return (ft_perror(-1, ft_strsjoin((char *[]){"Unexpected char near ",
					arg, "in id '", id, "'.", NULL}), 1), *err = 1, VOID);
	if (*value < limits[0])
		return (ft_perror(-1, ft_strsjoin((char *[]){"Expected a positive \
number bigger than ", lim_str[0], " for identifier '", id ,"'.", NULL}), 1),
				*err = 1, VOID);
}
