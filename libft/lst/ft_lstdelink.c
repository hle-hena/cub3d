/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lst_delink.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 09:44:33 by hle-hena          #+#    #+#             */
/*   Updated: 2025/02/07 09:44:58 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	ft_lstdelink(t_list **prev, t_list **link, void (*del)(void *))
{
	if (!*link)
		return ;
	if (*prev == *link)
		*prev = (*prev)->next;
	else
		(*prev)->next = (*link)->next;
	ft_lstdelone(*link, del);
}
