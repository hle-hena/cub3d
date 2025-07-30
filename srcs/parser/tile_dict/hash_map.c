/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 13:03:11 by hle-hena          #+#    #+#             */
/*   Updated: 2025/07/30 11:43:41 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

t_img	**get_hash_table(void)
{
	static t_img	*hash_table[HASH_SIZE];

	return (hash_table);
}

unsigned long	hash_str(const char *str)
{
	unsigned long	hash;

	hash = 5381;
	while (*str)
		hash = ((hash << 5) + hash) + *str++;
	return (hash % HASH_SIZE);
}

void	load_new_hash(t_img **hash_table, unsigned int i, char *path, int *err)
{
	hash_table[i] = ft_calloc(1, sizeof(t_img));
	if (!hash_table)
		return ;
	hash_table[i]->img = mlx_xpm_file_to_image(get_data()->mlx, path,
		&hash_table[i]->width, &hash_table[i]->height);
	if (!hash_table[i]->img)
		return (ft_perror(-1, ft_strsjoin((char *[]){"An error happened during \
the opening of the file '", path, "'.", NULL}), 1), *err = 1, VOID);
	hash_table[i]->data = (int *)mlx_get_data_addr(hash_table[i]->img,
		&hash_table[i]->bpp, &hash_table[i]->size_line, &hash_table[i]->endian);
	hash_table[i]->bpp /= 8;
	hash_table[i]->size_line /= hash_table[i]->bpp;
	hash_table[i]->path = ft_strdup(path);
	hash_table[i]->endian = 1;
	if (!hash_table[i]->path)
		(ft_perror(-1, "Internal error: malloc.", 0), *err = 1);
}

t_img	*get_img_hash(char *path, int *err)
{
	t_img			**hash_table;
	unsigned int	i;
	unsigned int	copy;

	i = hash_str(path);
	copy = i;
	hash_table = get_hash_table();
	while (hash_table[i] != NULL)
	{
		if (ft_strncmp(hash_table[i]->path, path, ft_strlen(path)) == 0)
			return (hash_table[i]);
		i = (i + 1) % HASH_SIZE;
		if (i == copy)
			return (NULL);
	}
	load_new_hash(hash_table, i, path, err);
	if (!hash_table[i] || *err)
		return (NULL);
	return (hash_table[i]);
}
