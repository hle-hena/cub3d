/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   retrieve_map_info_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 16:26:41 by hle-hena          #+#    #+#             */
/*   Updated: 2025/04/07 16:36:07 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	ft_atoi_err(char *arg, int *index)
{
	int		i;
	int		count;
	int		nb;
	char	*max;

	if (!arg)
		return (-1);
	i = -1;
	count = 0;
	nb = 0;
	max = "256";
	while (ft_isspace(*arg))
		arg++;
	if (!*arg)
		return (-1);
	while (*arg && max[++i] && ft_isdigit(*arg))
	{
		while (i == 0 && *arg == '0' && ft_isdigit(*(arg + 1)))
			arg++;
		if (*arg > max[i])
			count ++;
		nb = nb * 10 + (*arg - '0');
		(arg++, (*index)++);
	}
	return (ft_tern_int((i == 2 && count), -1, nb));
}

int	has_all_info(t_map	**map)
{
	if (!(*map)->img_ea.img || !(*map)->img_we.img || !(*map)->img_so.img
		|| !(*map)->img_no.img)
		return (0);
	if ((*map)->ceiling.bl == -1 || (*map)->floor.bl = -1)
		return (0);
	return (1);
}