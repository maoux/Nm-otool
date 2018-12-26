/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm_print.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heynard <heynard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/18 16:10:49 by heynard           #+#    #+#             */
/*   Updated: 2018/12/24 18:40:39 by heynard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

static char				get_type_sect(t_s_file *file, t_s_symbol *current)
{
	if (file->text_nsect == NO_SECT && file->data_nsect == NO_SECT
		&& file->bss_nsect == NO_SECT)
		return ('s');
	if (current->sect == file->text_nsect)
		return ('t');
	else if (current->sect == file->data_nsect)
		return ('d');
	else if (current->sect == file->bss_nsect)
		return ('b');
	return ('s');
}

static char				get_type_undf(t_s_file *file, t_s_symbol *current)
{
	char				c;

	c = 'u';
	if (file->format == MACHO_32BITS)
	{
		if (current->value.val_32 != 0)
			c = 'c';
	}
	else
	{
		if (current->value.val_64 != 0)
			c = 'c';
	}
	return (c);
}

static char				get_type(t_s_file *file, t_s_symbol *current)
{
	char				c;

	if (current->type & N_STAB)
		c = '-';
	else
	{
		if ((current->type & N_TYPE) == N_UNDF)
			c = get_type_undf(file, current);
		else if ((current->type & N_TYPE) == N_PBUD)
			c = 'u';
		else if ((current->type & N_TYPE) == N_ABS)
			c = 'a';
		else if ((current->type & N_TYPE) == N_SECT)
			c = get_type_sect(file, current);
		else if ((current->type & N_TYPE) == N_INDR)
			c = 'i';
		else
			c = '?';
		if ((current->type & N_EXT) && c != '?')
			c = c - 32;
	}
	return (c);
}

void					print_lst_sym_u(t_s_file *file, t_s_symbol *l,
										const int nfiles)
{
	unsigned char		c;

	print_file_name(file, nfiles);
	while (l)
	{
		c = get_type(file, l);
		if (c == 'U' || c == 'u')
			ft_printf("%s\n", l->name);
		else if (!(file->options & FLAG_u))
			ft_printf("%s\n", l->name);
		l = l->next;
	}
}

void					print_lst_sym(t_s_file *file, t_s_symbol *l,
										const int nfiles)
{
	unsigned char		c;

	print_file_name(file, nfiles);
	while (l)
	{
		c = get_type(file, l);
		if (file->format == MACHO_32BITS && c != '-')
			nm_print_32(file, l, c);
		else if (c != '-')
			nm_print_64(file, l, c);
		l = l->next;
	}
}
