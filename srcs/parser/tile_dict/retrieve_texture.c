/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   retrieve_texture.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 18:13:11 by hle-hena          #+#    #+#             */
/*   Updated: 2025/04/23 14:40:58 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	retrieve_texture_path(t_img **img, char *line, int *err)
{
	char			*extension;

	extension = ft_strrchr(line, '.');
	if (!extension)
		return (ft_perror(-1, ft_strsjoin((char *[]){"Invalid usage. Missing \
extension in asset ", line, ".", NULL}), 1), *err = 1, VOID);
	if (ft_strncmp(".xpm", extension, 5))
		return (ft_perror(-1, ft_strsjoin((char *[]){"Invalid usage. Assets \
should be in .xpm. Got ", line, ".", NULL}), 1), *err = 1, VOID);
	*img = get_img_hash(line, err);
	if (!*img)
		(ft_perror(-1, "Something went wrong retrieving the image in the \
hash_map.", 0), *err = 1);
}

void	retrieve_texture(t_img **texture, char *arg, int *err, char *id)
{
	if (*texture)
		return (ft_perror(-1, ft_strsjoin((char *[]){"Duplicate of identifier ",
						id, ".", NULL}), 1), *err = 1, VOID);
	if (ft_strchr(arg, '/'))
		retrieve_texture_path(texture, arg, err);
	else
		retrieve_texture_color(texture, arg, err);
}
