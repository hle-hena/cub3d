/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_parser.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 16:17:06 by hle-hena          #+#    #+#             */
/*   Updated: 2025/04/04 16:31:10 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	clean_map(void)
{
	t_map	*map;

	map = get_map();
	ft_del((void **)&map->path_ea);
	ft_del((void **)&map->path_we);
	ft_del((void **)&map->path_so);
	ft_del((void **)&map->path_no);
	ft_del((void **)&map->path_no);
	ft_free_tab(map->matrix, map->len);
}