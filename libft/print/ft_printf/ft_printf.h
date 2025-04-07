/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 13:48:04 by hle-hena          #+#    #+#             */
/*   Updated: 2025/01/13 13:52:00 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include "../../libft.h"
# include <stdarg.h>

# define HEX_MAJ "0123456789ABCDEF"
# define HEX_MIN "0123456789abcdef"

typedef struct s_param
{
	int		flags;
	int		width;
	int		precision;
	char	placeholder;
}	t_param;

typedef struct s_count
{
	int	i;
	int	j;
	int	k;
	int	len;
	int	temp;
}	t_count;

typedef struct s_nb
{
	unsigned long	nb;
	int				sign;
	int				nblen;
	int				print;
	int				pad;
	char			*base;
}	t_nb;

typedef struct s_str
{
	char	*str;
	char	let;
	int		print;
}	t_str;

int				handle_args(t_param args, va_list ap, int fd);
int				handle_digits(t_param, va_list ap, int fd);
int				handle_str(t_param args, va_list ap, int fd);
int				handle_percent(int fd);
int				print_null(t_param args, char *str, int fd);

#endif