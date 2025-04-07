/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstremove_if.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 18:01:30 by hle-hena          #+#    #+#             */
/*   Updated: 2025/02/12 18:28:36 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

static void	remove_node(t_list **node, void (*del)(void *)) {
	t_list	*temp;

	temp = *node;
	*node = (*node)->next;
	del(temp->content);
	del(temp);
}

void	ft_lstremove_if(t_list **head, int (*cnd)(void *, void *),
	void (*del)(void *), char *to_find)
{
	t_list *current;
	t_list *prev;

	current = *head;
	prev = NULL;
	while (current != NULL && cnd(current->content, to_find))
	{
		remove_node(head, del);
		current = *head;
	}
	while (current != NULL)
	{
		if (cnd(current->content, to_find))
		{
			remove_node(&current, del);
			prev->next = current;
		}
		else
		{
			prev = current;
			current = current->next;
		}
	}
}
