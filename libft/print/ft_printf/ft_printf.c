/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 13:48:27 by hle-hena          #+#    #+#             */
/*   Updated: 2025/01/13 13:49:28 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
'#' : 1
'+' : 2
' ' : 4
'0' : 8
'-' : 16
*/

#include "ft_printf.h"

void	init_args(t_param *args)
{
	args->flags = 0;
	args->width = 0;
	args->precision = -2;
	args->placeholder = 'q';
}

int	parse(t_param *args, char *str)
{
	int		i;
	int		flag_val;

	i = 1;
	while (str[i] && ft_strchr("#+ 0-", str[i]))
	{
		flag_val = (str[i] == '#') * 1 + (str[i] == '+') * 2 + (str[i] == ' '
				) * 4 + (str[i] == '0') * 8 + (str[i] == '-') * 16;
		i++;
		if (args->flags & flag_val)
			continue ;
		args->flags += flag_val;
	}
	(*args).width = ft_atoi(&str[i]);
	i += ft_numlen((*args).width) * ((*args).width != 0);
	if (str[i] == '.')
	{
		i ++;
		(*args).precision = ft_atoi(&str[i]) + (str[i] == '*') * (-1);
		while ((str[i] >= '0' && str[i] <= '9') || str[i] == '*')
			i++;
	}
	(*args).placeholder = str[i];
	return (i);
}

int	ft_printf(const char *str, ...)
{
	va_list	ap;
	t_param	args;
	int		i;
	int		written;
	int		fd;

	fd = 1;
	va_start(ap, str);
	init_args(&args);
	i = -1;
	written = 0;
	while (str[++i])
	{
		if (str[i] == '%')
		{
			i += parse(&args, (char *)(&str[i]));
			written += handle_args(args, ap, fd);
			init_args(&args);
		}
		else
			written += ft_putchar_fd(str[i], 1);
	}
	return (va_end(ap), written);
}
