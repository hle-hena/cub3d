/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 11:51:28 by hle-hena          #+#    #+#             */
/*   Updated: 2025/05/19 11:57:36 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

t_list	*ft_lstchr(t_list *lst, void *to_find, int (*cmp)(void *, void *))
{
	while (lst)
	{
		if (cmp(lst->content, to_find))
			return (lst);
		lst = lst->next;
	}
	return (NULL);
}
