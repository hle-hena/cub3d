/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   retrieve_texture.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 18:13:11 by hle-hena          #+#    #+#             */
/*   Updated: 2025/04/25 09:45:16 by hle-hena         ###   ########.fr       */
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

void	get_arg(char *line, char **arg, char **reflectance, int *err)
{
	char	**splited;
	char	*temp;
	int		len;
	int		i;

	splited = ft_split(line, ' ');
	if (!splited)
		return (*err = 1, ft_perror(-1, "Internal error: malloc", 0), VOID);
	len = ft_strslen(splited);
	temp = NULL;
	if (len > 1)
	{
		i = -1;
		while (++i < len - 1)
		{
			*arg = ft_strjoin(temp, splited[i]);
			ft_del((void **)&temp);
			if (!*arg)
				return (*err = 1, ft_perror(-1, "Internal error: malloc", 0),
					ft_free_tab((void **)splited, len), VOID);
			temp = *arg;
		}
		*reflectance = ft_strdup(splited[i]);
		if (!*reflectance)
			return (*err = 1, ft_perror(-1, "Internal error: malloc", 0),
				ft_free_tab((void **)splited, len), ft_del((void **)&(*arg)), VOID);
	}
	else
	{
		*arg = ft_strdup(splited[0]);
		if (!*arg)
			return (*err = 1, ft_perror(-1, "Internal error: malloc", 0),
				ft_free_tab((void **)splited, len), VOID);
		*reflectance = NULL;
	}
	ft_free_tab((void **)splited, len);
}

void	retrieve_texture(t_text *texture, char *line, int *err, char *id)
{
	char	*arg;
	char	*reflectance;

	if (texture->img)//Should see if I need to NULL it at start.
		return (ft_perror(-1, ft_strsjoin((char *[]){"Duplicate of identifier ",
						id, ".", NULL}), 1), *err = 1, VOID);
	get_arg(line, &arg, &reflectance, err);
	if (*err)
		return ;
	if (ft_strchr(arg, '/'))
		retrieve_texture_path(&texture->img, arg, err);
	else
		retrieve_texture_color(&texture->img, arg, err);
	if (reflectance)
		retrieve_texture_val(texture, reflectance, err);
}
