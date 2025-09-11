/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 13:47:48 by hle-hena          #+#    #+#             */
/*   Updated: 2025/01/13 13:53:32 by hle-hena         ###   ########.fr       */
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

int	print_str(t_param args, t_str str, int fd)
{
	int	len;

	if (args.placeholder == 'c')
		return (ft_putchar_fd(str.let, fd));
	len = 0;
	while (len < str.print)
		len += ft_putchar_fd(*(str.str)++, fd);
	return (len);
}

t_str	create_str(t_param args, va_list ap)
{
	t_str	str;
	int		strlen;

	str.str = NULL;
	if (args.placeholder == 'c')
	{
		str.let = va_arg(ap, int);
		str.print = 1;
	}
	else if (args.placeholder == 's')
	{
		str.str = va_arg(ap, char *);
		strlen = ft_strlen(str.str);
		if (args.precision == -2)
			str.print = strlen;
		else
			str.print = ft_tern_int(args.precision < strlen, args.precision,
					strlen);
	}
	return (str);
}

int	handle_str(t_param args, va_list ap, int fd)
{
	int		len;
	t_str	str;

	len = 0;
	str = create_str(args, ap);
	if (args.placeholder == 's' && !str.str)
		return (print_null(args, "(null)", fd));
	if (args.flags & 16)
		len += print_str(args, str, fd);
	while (len < args.width - str.print * (!(args.flags & 16)))
		len += ft_putchar_fd(' ', fd);
	if (!(args.flags & 16))
		len += print_str(args, str, fd);
	return (len);
}

int	handle_percent(int fd)
{
	return (ft_putchar_fd('%', fd));
}
