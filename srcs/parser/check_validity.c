/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_validity.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 16:22:39 by hle-hena          #+#    #+#             */
/*   Updated: 2025/04/08 17:20:32 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"


#include <stdio.h>
#include <stdlib.h>

void	push(t_flood **stack, int x, int y)
{
	t_flood	*new;

	new = malloc(sizeof(t_flood));
	if (!new)
		return;
	new->point.x = x;
	new->point.y = y;
	new->next = *stack;
	*stack = new;
}

t_point	pop(t_flood **stack)
{
	t_flood	*top;
	t_point	point;

	top = *stack;
	point = top->point;
	*stack = top->next;
	ft_del((void **)&top);
	return (point);
}

void	clear_stack(t_flood **stack)
{
	t_flood	*temp;

	while (*stack)
	{
		temp = *stack;
		*stack = temp->next;
		ft_del((void **)&temp);
	}
}

void flood_fill(t_map *map, int start_x, int start_y)
{
	t_flood	*stack;
	t_flood	*reverse;
	t_point	point;

	stack = NULL;
	reverse = NULL;
	push(&stack, start_x, start_y);
	while (stack)
	{
		point = pop(&stack);
		if (point.x < 0 || point.x >= map->wid || point.y < 0 || point.y >= map->len)
			continue;
		if (map->matrix[point.y][point.x] != '0')
			continue;
		map->matrix[point.y][point.x] = '1';
		push(&stack, point.x + 1, point.y);
		push(&stack, point.x - 1, point.y);
		push(&stack, point.x, point.y + 1);
		push(&stack, point.x, point.y - 1);
		push(&reverse, point.x, point.y);
	}
}


void	check_validity(t_map *map, int *err)
{
	if (*err)
		return ;
	
}