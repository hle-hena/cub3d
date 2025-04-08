/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 16:05:37 by hle-hena          #+#    #+#             */
/*   Updated: 2025/04/08 17:18:39 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

t_data	*get_data(void)
{
	static t_data	data = (t_data){0};

	return (&data);
}

void print_map(t_map *map)
{
	printf("Map Dimensions: %d x %d\n", map->wid, map->len);

	// Print map matrix
	printf("Map Matrix:\n");
	for (int y = 0; y < map->len; y++)
	{
		printf("%s\n", map->matrix[y]);
	}

	// Image info (only width/height since img is a void pointer)
	printf("\nImage Info:\n");
	printf("North:  %dx%d\n", map->img_no.width, map->img_no.height);
	printf("South:  %dx%d\n", map->img_so.width, map->img_so.height);
	printf("West:   %dx%d\n", map->img_we.width, map->img_we.height);
	printf("East:   %dx%d\n", map->img_ea.width, map->img_ea.height);

	// Floor and ceiling colors
	printf("\nFloor Color:   R:%d G:%d B:%d\n", map->floor.re, map->floor.gr, map->floor.bl);
	printf("Ceiling Color: R:%d G:%d B:%d\n", map->ceiling.re, map->ceiling.gr, map->ceiling.bl);

	// Player position
	printf("\nPlayer Position: x:%.2f y:%.2f\n", map->player.x, map->player.y);
}

int	main(int ac, char **av)
{
	t_data	*data;

	data = get_data();
	data->mlx = mlx_init();
	data->map = load_map(ac, av);
	print_map(data->map);
	clean_data();
	return (0);
}