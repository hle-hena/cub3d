/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 10:45:46 by hle-hena          #+#    #+#             */
/*   Updated: 2024/12/11 17:33:35 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

static void	del(char **content)
{
	if (*content)
	{
		free(*content);
		*content = NULL;
	}
}

static char	*format_output(char *res, char *mem, int rv)
{
	int		i;
	char	*line;

	if (ft_strchr(res, '\n'))
	{
		i = 0;
		line = ft_strchr(res, '\n');
		while (line[++i])
			mem[i - 1] = line[i];
		mem[i - 1] = 0;
	}
	i = 0;
	while (res[i] && res[i] != '\n')
		i++;
	i -= (!res[i]);
	line = ft_calloc(i + 2, sizeof(char));
	if (!line)
		return (del(&res), NULL);
	while (--i >= -1)
		line[i + 1] = res[i + 1];
	if (*res == 0 && rv == 0)
		return (del(&res), del(&line), NULL);
	return (del(&res), line);
}

int	read_next_line(int fd, char **res, char **buffer)
{
	char	*temp;
	int		rv;

	while (1)
	{
		rv = read(fd, *buffer, BUFFER_SIZE);
		(*buffer)[rv] = 0;
		temp = ft_strjoin(*res, *buffer);
		if (!temp)
			return (del(res), del(buffer), -1);
		del(res);
		*res = temp;
		if (ft_strchr(*res, '\n') || rv != BUFFER_SIZE)
			break ;
	}
	return (rv);
}

char	*get_next_line(int fd)
{
	static char	mem[1024][BUFFER_SIZE + 1];
	char		*res;
	char		*temp;
	int			rv;
	char		*buffer;

	if (fd < 0 || fd > 1024)
		return (NULL);
	buffer = ft_calloc(BUFFER_SIZE + 1, sizeof(char));
	res = ft_calloc(1, sizeof(char));
	if (!buffer || !res || read(fd, buffer, 0) < 0
		|| BUFFER_SIZE <= 0)
		return (del(&res), del(&buffer), ft_bzero(mem[fd], BUFFER_SIZE + 1),
			NULL);
	temp = ft_strjoin(mem[fd], res);
	if (!temp)
		return (del(&res), del(&buffer), NULL);
	ft_bzero(mem[fd], BUFFER_SIZE + 1);
	del(&res);
	res = temp;
	rv = read_next_line(fd, &res, &buffer);
	if (rv == -1)
		return (del(&res), del(&buffer), NULL);
	return (del(&buffer), format_output(res, mem[fd], rv));
}
