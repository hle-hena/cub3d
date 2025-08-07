/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_flight.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 11:58:38 by hle-hena          #+#    #+#             */
/*   Updated: 2025/08/07 15:00:55 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

t_flight	*new_flight(t_vec normal)
{
	t_flight	*flight;

	flight = malloc(sizeof(t_flight));
	if (!flight)
		return (NULL);
	flight->color = 0;
	flight->emittance = 0;
	flight->normal = normal;
	flight->ibuffer = 0;
	return (flight);
}

t_flight	*find_flight(t_data *data, t_point pos, t_wpath wall)
{
	t_tlight	*tlight;
	t_list		*lst;
	t_flight	*flight;

	tlight = (data->lmap.lmap + pos.x + pos.y * data->lmap.wid);
	lst = ft_lstchr(tlight->flight, &wall.normal, 0.1f, is_correct_flight);
	if (!lst)
	{
		flight = new_flight(wall.normal);
		add_link(&tlight->flight, flight);
	}
	else
		flight = (t_flight *)lst->content;
	return (flight);
}
