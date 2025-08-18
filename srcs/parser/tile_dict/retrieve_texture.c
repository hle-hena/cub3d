/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   retrieve_texture.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 18:13:11 by hle-hena          #+#    #+#             */
/*   Updated: 2025/08/18 14:02:38 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	retrieve_texture_path(t_img **img, char *line, int *err)
{
	char			*extension;

	extension = ft_strrchr(line, '.');
	if (!extension)
		return (*err = 1, ft_perror(-1, ft_strsjoin((char *[]){"Invalid usage. \
Missing extension in asset ", line, ".", NULL}), 1));
	if (ft_strncmp(".xpm", extension, 5))
		return (*err = 1, ft_perror(-1, ft_strsjoin((char *[]){"Invalid usage. \
Assets should be in .xpm. Got ", line, ".", NULL}), 1));
	*img = get_img_hash(line, err);
	if (!*img)
		(ft_perror(-1, "Something went wrong retrieving the image in the \
hash_map.", 0), *err = 1);
}

void	get_arg(char **splited, char **arg, char **reflectance, int *err)
{
	int		len;

	if (!splited)
		return (*err = 1, ft_perror(-1, "Internal error: malloc", 0));
	len = ft_strslen(splited);
	if (len > 2)
		return (*err = 1, ft_perror(-1, "Please, don't input a texture that \
contains a :.", 0));
	if (len > 1)
	{
		*arg = ft_strdup(splited[0]);
		*reflectance = ft_strdup(splited[1]);
		if (!*reflectance || !arg)
			return (*err = 1, ft_perror(-1, "Internal error: malloc", 0),
				ft_free_tab((void **)splited, len), ft_del((void **)&(*arg)),
				ft_del((void **)&(*reflectance)));
		return (ft_free_tab((void **)splited, len));
	}
	*arg = ft_strdup(splited[0]);
	*reflectance = NULL;
	ft_free_tab((void **)splited, len);
	if (!*arg)
		return (*err = 1, ft_perror(-1, "Internal error: malloc", 0));
}

void	retrieve_texture(t_text *texture, char *line, int *err, char *id)
{
	char	*arg;
	char	*reflectance;

	if (texture->img)
		return (*err = 1, ft_perror(-1, ft_strsjoin((char *[]){"Duplicate of \
identifier ", id, ".", NULL}), 1));
	get_arg(ft_split(line, ':'), &arg, &reflectance, err);
	if (*err)
		return (ft_del((void **)&arg), ft_del((void **)&reflect_light));
	if (ft_strchr(arg, '/'))
		retrieve_texture_path(&texture->img, arg, err);
	else
		retrieve_texture_color(&texture->img, arg, err);
	ft_del((void **)&arg);
	if (*err)
		return ;
	if (reflectance)
		retrieve_texture_val(texture, reflectance, err);
	ft_del((void **)&reflectance);
}
