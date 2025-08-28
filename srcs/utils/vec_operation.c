/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_operation.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 13:45:19 by hle-hena          #+#    #+#             */
/*   Updated: 2025/08/28 10:11:10 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

t_vec	vec_sub(t_vec a, t_vec b)
{
	return ((t_vec){a.x - b.x, a.y - b.y});
}

t_vec	vec_add(t_vec a, t_vec b)
{
	return ((t_vec){a.x + b.x, a.y + b.y});
}

float	vec_dot(t_vec a, t_vec b)
{
	return (a.x * b.x + a.y * b.y);
}

float	vec_cross(t_vec a, t_vec b)
{
	return (a.x * b.y - a.y * b.x);
}

t_vec	vec_scale(t_vec a, float s)
{
	return ((t_vec){a.x * s, a.y * s});
}
