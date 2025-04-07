/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_args.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 13:47:31 by hle-hena          #+#    #+#             */
/*   Updated: 2025/01/13 13:50:40 by hle-hena         ###   ########.fr       */
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

int	print_null(t_param args, char *str, int fd)
{
	int	len;
	int	strlen;
	int	shouldprint;

	len = 0;
	strlen = ft_strlen(str);
	shouldprint = ((args.placeholder == 's'
				&& (args.precision >= strlen
					|| args.precision == -2)) || args.placeholder == 'p');
	if (args.flags & 16 && shouldprint)
		len += ft_putstr_fd(str, fd);
	while (len < args.width - strlen * (!(args.flags & 16)) * shouldprint)
		len += ft_putchar_fd(' ', fd);
	if (!(args.flags & 16) && shouldprint)
		len += ft_putstr_fd(str, fd);
	return (len);
}

void	check_invalid_args(t_param *args)
{
	if (args->precision != -2 && args->flags & 8)
		args->flags -= 8;
	if (args->flags & 16 && args->flags & 8)
		args->flags -= 8;
	if (args->flags & 4 && args->flags & 2)
		args->flags -= 4;
}

int	handle_args(t_param args, va_list ap, int fd)
{
	if (args.precision == -1)
		args.precision = va_arg(ap, int);
	check_invalid_args(&args);
	if (args.placeholder == '%')
		return (handle_percent(fd));
	else if (ft_strchr("cs", args.placeholder))
		return (handle_str(args, ap, fd));
	else if (ft_strchr("uidpxX", args.placeholder))
		return (handle_digits(args, ap, fd));
	return (0);
}
