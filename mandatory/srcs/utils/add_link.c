/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_link.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 13:07:39 by hle-hena          #+#    #+#             */
/*   Updated: 2025/04/08 17:02:26 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	add_link(t_list **lst, char *content)
{
	t_list	*link;

	if (!content)
		ft_perror(1, "cub3d: Internal error: malloc", clean_data());
	link = ft_lstnew(content);
	if (!link)
	{
		ft_del((void **)&content);
		ft_perror(1, "cub3d: Internal error: malloc.", clean_data());
	}
	ft_lstadd_back(lst, link);
}
