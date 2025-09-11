/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 18:03:30 by hle-hena          #+#    #+#             */
/*   Updated: 2025/04/08 17:00:39 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void **))
{
	t_list	*temp;

	while (*lst)
	{
		temp = (*lst)->next;
		if (del)
		{
			del((void **)&(*lst)->content);
			del((void **)lst);
		}
		*lst = temp;
	}
	*lst = 0;
}
