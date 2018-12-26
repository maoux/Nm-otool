/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_memory.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heynard <heynard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/08 22:10:47 by heynard           #+#    #+#             */
/*   Updated: 2018/06/08 22:10:47 by heynard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

/*
** args :		long int
** function :	display recursivly the int on STDOUT in hexa
** return :		void
*/

static void		ft_print_hexa(unsigned long int value)
{
	if (value <= 0)
		return ;
	ft_print_hexa(value / 16);
	if (value % 16 <= 9)
		ft_putnbr(value % 16);
	else
		ft_putchar('a' + (value % 16 - 10));
}

/*
** args :		long int
** function :	print on STDOUT the argument with padding to have a width of 16
**				bits. Padding is made of 0 char
** return :		void
*/

void			ft_print_memory(unsigned long int value)
{
	uint8_t		value_length;
	uint64_t	tmp;

	value_length = 0;
	tmp = value;
	while (tmp > 0)
	{
		value_length++;
		tmp /= 16;
	}
	while (++value_length <= 16)
		ft_putchar('0');
	ft_print_hexa(value);
	ft_putchar(' ');
}
