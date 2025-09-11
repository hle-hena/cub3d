/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersect.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 11:43:04 by hle-hena          #+#    #+#             */
/*   Updated: 2025/08/28 10:11:10 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

t_inter	intersect_switch(t_vec origin, t_vec dir, t_wpath path_to_inter)
{
	if (path_to_inter.mode == 1)
		return (intersect_sarc(origin, dir, path_to_inter));
	return (intersect_sseg(origin, dir, path_to_inter));
}
