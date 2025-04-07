/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstcountf.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 13:17:57 by hle-hena          #+#    #+#             */
/*   Updated: 2024/11/25 13:20:23 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

int	ft_lstcountf(t_list *lst, int (*f)(t_list *))
{
	int	count;

	count = 0;
	if (!lst)
		return (count);
	while (lst->next)
	{
		if (f(lst))
			count++;
		lst = lst->next;
	}
	return (count);
}
