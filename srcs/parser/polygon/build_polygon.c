/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_polygon.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 10:55:56 by hle-hena          #+#    #+#             */
/*   Updated: 2025/08/12 16:24:20 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	print_graph(t_graph *graph, char *letter)
{
	int	i;

	i = -1;
	printf("This is the graph of %s:\n", letter);
	while (++i < graph->nb_links)
	{
		if (graph->links[i].type == 0)
		{
			printf("\tA line is connecting {%.12f, %.12f} to {%.12f, %.12f}\n",
				graph->links[i].start->coo.x, graph->links[i].start->coo.y,
				graph->links[i].end->coo.x, graph->links[i].end->coo.y);
		}
		else
		{
			printf("\tAn arc is connecting {%.12f, %.12f} to {%.12f, %.12f}\
, with {%.12f, %.12f} as center\n",
				graph->links[i].start->coo.x, graph->links[i].start->coo.y,
				graph->links[i].end->coo.x, graph->links[i].end->coo.y,
				graph->links[i].center.x, graph->links[i].center.y);
		}
	}
}

int	copy_graph(t_graph *outer_face, t_tile *tile, float perimeter)
{
	int		i;
	t_wpath	*temp;
	float	perim;

	i = -1;
	perim = 0;
	ft_lstclear(&tile->wpath, ft_del);
	tile->wpath = NULL;
	while (++i < outer_face->nb_links)
	{
		temp = malloc(sizeof(t_wpath));
		if (!temp)
			return (ft_perror(-1, "Internal error: malloc.", 0), 1);
		*temp = (t_wpath){.start = outer_face->links[i].start->coo, .pos = 0,
			.end = outer_face->links[i].end->coo, .pos_start = perim,
			.texture = tile->wall, .mode = outer_face->links[i].type};
		temp->center = outer_face->links[i].center;
		temp->reflectance = outer_face->links[i].reflectance;
		temp->normal = (t_vec){0};
		temp->run_forward = outer_face->links[i].run_forward;
		perim += perimeter_link(&outer_face->links[i]) / perimeter;
		temp->pos_end = perim;
		ft_lstadd_back(&tile->wpath, ft_lstnew(temp));
	}
	return (0);
}

int	build_polygon(t_tile *tile, char id)
{
	char	letter[2];
	t_graph	*graph;
	t_graph	*outer_face;
	float	perimeter;

	if (((t_wpath *)tile->wpath->content)->texture.img)
		return (0);
	letter[0] = id;
	letter[1] = 0;
	if (ft_lstsize(tile->wpath) >= 32)
		return (ft_perror(-1, ft_strsjoin((char *[]){"Too many paths for tile \
", letter, ". Expected at most 32 paths.", NULL}), 0), 1);
	graph = malloc(sizeof(t_graph));
	if (!graph)
		return (ft_perror(-1, "Internal error: malloc.", 0), 1);
	if (build_primitive_graph(tile->wpath, graph))
		return (ft_del((void **)&graph), 1);
	if (grow_graph(graph))
		return (ft_del((void **)&graph), 1);
	outer_face = retrieve_outer_face(graph, &perimeter);
	if (outer_face)
		copy_graph(outer_face, tile, perimeter);
	ft_del((void **)&graph);
	ft_del((void **)&outer_face);
	return (0);
}

int	build_polygons(void)
{
	t_tile	**tiles;
	int		i;

	tiles = get_tile_dict();
	i = -1;
	while (++i < 255)
	{
		if (!tiles[i])
			continue ;
		if (build_polygon(tiles[i], i))
			return (1);
	}
	return (0);
}
