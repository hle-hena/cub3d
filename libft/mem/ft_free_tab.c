/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_tab.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 10:56:44 by hle-hena          #+#    #+#             */
/*   Updated: 2025/04/08 10:15:58 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	ft_free_tab(void **tab, size_t size)
{
	void	**temp;
	size_t	i;

	i = -1;
	if (!tab)
		return ;
	temp = tab;
	while (++i < size)
		ft_del((void **)&(*tab++));
	ft_del((void **)&temp);
}
