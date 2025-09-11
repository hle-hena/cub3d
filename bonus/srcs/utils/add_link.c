/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_link.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 13:07:39 by hle-hena          #+#    #+#             */
/*   Updated: 2025/08/27 17:29:38 by gakarbou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void	add_link(t_list **lst, void *content)
{
	t_list	*link;

	if (!content)
		ft_perror(1, "cub3D: Internal error: malloc", clean_data());
	link = ft_lstnew(content);
	if (!link)
	{
		ft_del((void **)&content);
		ft_perror(1, "cub3D: Internal error: malloc.", clean_data());
	}
	ft_lstadd_back(lst, link);
}
