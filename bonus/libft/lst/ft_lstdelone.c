/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdelone.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 12:27:47 by hle-hena          #+#    #+#             */
/*   Updated: 2025/04/08 17:01:14 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	ft_lstdelone(t_list *lst, void (*del)(void **))
{
	del((void **)&lst->content);
	del((void **)&lst);
}
