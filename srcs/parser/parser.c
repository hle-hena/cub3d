/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 16:16:19 by hle-hena          #+#    #+#             */
/*   Updated: 2025/08/27 17:26:16 by gakarbou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

t_map	*get_map(void)
{
	static t_map	map = (t_map){0};

	return (&map);
}

int	get_map_fd(char *map_name)
{
	char	*extension;
	char	trash_read;
	int		map_fd;

	extension = ft_strrchr(map_name, '.');
	if (!extension)
		ft_perror(1, "Invalid usage. File should be in .cub", clean_data());
	if (ft_strncmp(".cub", extension, 5))
		ft_perror(1, "Invalid usage. File should be in .cub", clean_data());
	map_fd = open(map_name, O_RDONLY, 0777);
	if (map_fd < 0)
		ft_perror(1, "File not found.", clean_data());
	if (read(map_fd, &trash_read, 0) < 0)
		ft_perror(1, "Invalid usage. Expecting a file, not a dir.",
			clean_data());
	return (map_fd);
}

t_map	*load_map(int ac, char **av)
{
	t_map	*map;
	char	*temp;
	int		map_fd;
	int		err;

	err = 0;
	if (ac != 2)
		ft_perror(1, "Invalid usage. Correct :\n\t./cub3D map_name.cub",
			clean_data());
	map_fd = get_map_fd(av[1]);
	map = get_map();
	ft_memset(get_tile_dict(), 0, 256 * sizeof(get_tile_dict()));
	map->void_char = ' ';
	map->replace_tile = map->void_char;
	map->player.y = -1;
	temp = retrieve_tile_dict(map, map_fd, &err);
	if (is_dict_full(map, err))
		return (ft_del((void **)&temp), NULL);
	if (build_polygons())
		return (ft_del((void **)&temp), NULL);
	retrieve_map(map, temp, map_fd, &err);
	if (is_map_valid(map, get_tile_dict(), err))
		return (NULL);
	return (close(map_fd), map);
}
