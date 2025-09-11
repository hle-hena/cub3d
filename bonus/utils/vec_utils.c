/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 13:46:23 by hle-hena          #+#    #+#             */
/*   Updated: 2025/09/11 14:50:20 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D_bonus.h"

float	vec_len2(t_vec a)
{
	return (a.x * a.x + a.y * a.y);
}

t_vec	vec_normalize(t_vec a)
{
	float	len;

	len = sqrtf(vec_len2(a));
	if (len < FLT_EPSILON)
		return ((t_vec){0, 0});
	return (vec_scale(a, 1.0f / len));
}
