/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   digits.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 13:48:20 by hle-hena          #+#    #+#             */
/*   Updated: 2025/01/13 13:52:29 by hle-hena         ###   ########.fr       */
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

int	print_sign(t_param args, t_nb nb, int fd)
{
	if (nb.sign)
		return (ft_putchar_fd('-', fd));
	else if (!nb.sign && args.flags & 2)
		return (ft_putchar_fd('+', fd));
	else if (!nb.sign && args.flags & 4)
		return (ft_putchar_fd(' ', fd));
	else if ((args.flags & 1 && args.placeholder == 'x' && nb.nb != 0)
		|| args.placeholder == 'p')
		return (ft_putstr_fd("0x", fd));
	else if (args.flags & 1 && args.placeholder == 'X' && nb.nb != 0)
		return (ft_putstr_fd("0X", fd));
	return (0);
}

int	print_digits(t_param args, t_nb nb, int fd)
{
	t_count	c;

	c.len = 0;
	while (c.len < args.precision - nb.nblen)
		c.len += ft_putchar_fd('0', fd);
	if ((nb.nb == 0 && args.precision == 0)
		&& ft_strchr("uidxX", args.placeholder))
		return (c.len);
	c.len += ft_putnbr_base_fd(nb.nb, nb.base, fd);
	return (c.len);
}

void	get_nb(t_param args, va_list ap, t_nb *nb)
{
	int	temp_nb;

	if (ft_strchr("uxX", args.placeholder))
		nb->nb = va_arg(ap, unsigned int);
	else if (args.placeholder == 'p')
		nb->nb = va_arg(ap, unsigned long int);
	else
	{
		temp_nb = va_arg(ap, int);
		nb->sign = temp_nb < 0;
		if (nb->sign)
			nb->nb = -(long)temp_nb;
		else
			nb->nb = temp_nb;
	}
}

t_nb	create_nb(t_param args, va_list ap)
{
	t_nb	nb;

	nb.sign = 0;
	get_nb(args, ap, &nb);
	nb.pad = ft_tern_int(args.flags & 8, '0', ' ');
	nb.base = "0123456789";
	if (ft_strchr("px", args.placeholder))
		nb.base = HEX_MIN;
	else if (args.placeholder == 'X')
		nb.base = HEX_MAJ;
	if ((nb.nb == 0 && args.precision == 0)
		&& ft_strchr("uidxX", args.placeholder))
		nb.nblen = 0;
	else
		nb.nblen = ft_numlen_base(nb.nb, nb.base)
			+ (args.placeholder == 'p') * 2;
	nb.print = ft_tern_int(nb.nblen > args.precision, nb.nblen, args.precision);
	if (ft_strchr("xX", args.placeholder))
		nb.print += ((args.flags) & 1) * 2;
	else if (ft_strchr("id", args.placeholder))
		nb.print += nb.sign + (!nb.sign && (args.flags & 2 || args.flags & 4));
	return (nb);
}

int	handle_digits(t_param args, va_list ap, int fd)
{
	t_count	c;
	t_nb	nb;

	c.len = 0;
	c.temp = 0;
	nb = create_nb(args, ap);
	if (nb.nb == 0 && args.placeholder == 'p')
		return (print_null(args, "(nil)", fd));
	if (args.flags & 16)
		c.len += print_sign(args, nb, fd) + print_digits(args, nb, fd);
	if (nb.pad == '0' && !(args.flags & 16))
		c.temp += print_sign(args, nb, fd);
	while (c.len < args.width - (nb.print * (!(args.flags & 16))))
		c.len += ft_putchar_fd(nb.pad, fd);
	if (nb.pad == ' ' && !(args.flags & 16))
		c.temp += print_sign(args, nb, fd);
	if (!(args.flags & 16))
		c.len += print_digits(args, nb, fd);
	return (c.len + c.temp);
}
