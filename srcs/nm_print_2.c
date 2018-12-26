/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm_print_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heynard <heynard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/24 18:38:53 by heynard           #+#    #+#             */
/*   Updated: 2018/12/24 18:41:13 by heynard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

void					print_file_name(t_s_file *file, const int nfiles)
{
	if (nfiles > 1 || file->narch > 1)
	{
		if (file->narch > 1)
		{
			if (file->ppc && file->format == MACHO_32BITS)
				ft_printf("\n%s (for architecture ppc):\n", file->name);
			else if (file->ppc && file->format == MACHO_64BITS)
				ft_printf("\n%s (for architecture ppc64):\n", file->name);
			else if (file->format == MACHO_32BITS)
				ft_printf("\n%s (for architecture i386):\n", file->name);
			else if (file->format == MACHO_64BITS)
				ft_printf("\n%s (for architecture x86_64):\n", file->name);
		}
		else if (file->is_archive)
			ft_printf("\n%s(%s):\n", file->name, file->obj_name);
		else if (nfiles > 1 && file->narch == 1)
			ft_printf("\n%s:\n", file->name);
	}
}

void					nm_print_32(t_s_file *file, t_s_symbol *l, char c)
{
	if ((c == 'u' || c == 'U' || c == '-' || c == '?')
		&& !(file->options & FLAG_U))
		ft_printf("         %c %s\n", c, l->name);
	else if (c != 'u' && c != 'U' && c != '-' && c != '?')
		ft_printf("%08zx %c %s\n", file->cigam ?
		swap_uint32(l->value.val_32) : l->value.val_32, c, l->name);
}

void					nm_print_64(t_s_file *file, t_s_symbol *l, char c)
{
	if ((c == 'u' || c == 'U' || c == '-' || c == '?')
		&& !(file->options & FLAG_U))
		ft_printf("                 %c %s\n", c, l->name);
	else if (c != 'u' && c != 'U' && c != '-' && c != '?')
		ft_printf("%016zx %c %s\n",
			file->cigam ? swap_uint64(l->value.val_64) :
			l->value.val_64, c, l->name);
}
