/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sym_parser.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heynard <heynard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/16 17:39:22 by heynard           #+#    #+#             */
/*   Updated: 2018/12/24 18:31:29 by heynard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

/*
** parse symtab and return a linked list of symbols with useful infos
** using lst_sym.c for lst building / freeing
** error self handled
** mach-o 32 / 64 compatible
** little / big endian compatible
*/

static t_s_symbol			*sym_parser_loop_64(t_s_file *file,
											struct symtab_command *lc,
											void *lst_sym,
											char *strtab)
{
	t_s_symbol				*my_lst_sym;
	struct nlist_64			*lst_sym_64;
	uint32_t				i;

	i = 0;
	my_lst_sym = NULL;
	lst_sym_64 = (struct nlist_64 *)lst_sym;
	while (i < (file->cigam ? swap_uint32(lc->nsyms) : lc->nsyms))
	{
		my_lst_sym = add_lst_sym(file, my_lst_sym,
			strtab + (file->cigam ? swap_uint32(lst_sym_64[i].n_un.n_strx)
			: lst_sym_64[i].n_un.n_strx), (void *)(&lst_sym_64[i]));
		i++;
	}
	return (my_lst_sym);
}

static t_s_symbol			*sym_parser_loop_32(t_s_file *file,
											struct symtab_command *lc,
											void *lst_sym,
											char *strtab)
{
	t_s_symbol				*my_lst_sym;
	struct nlist			*lst_sym_32;
	uint32_t				i;

	i = 0;
	my_lst_sym = NULL;
	lst_sym_32 = (struct nlist *)lst_sym;
	while (i < (file->cigam ? swap_uint32(lc->nsyms) : lc->nsyms))
	{
		my_lst_sym = add_lst_sym(file, my_lst_sym,
			strtab + (file->cigam ? swap_uint32(lst_sym_32[i].n_un.n_strx)
			: lst_sym_32[i].n_un.n_strx), (void *)(&lst_sym_32[i]));
		i++;
	}
	return (my_lst_sym);
}

static t_s_symbol			*sym_create_list_32(t_s_file *file,
											struct symtab_command *lc,
											char *strtab)
{
	struct nlist			*lst_sym_32;
	uint32_t				offset_cigam_cpt;

	offset_cigam_cpt = file->cigam ? swap_uint32(lc->symoff) : lc->symoff;
	lst_sym_32 = file->map + offset_cigam_cpt;
	if (!check_memory(file, (void *)lst_sym_32 + sizeof(struct nlist)
		* (file->cigam ? swap_uint32(lc->nsyms) : lc->nsyms)))
		return (NULL);
	offset_cigam_cpt = file->cigam ? swap_uint32(lc->stroff) : lc->stroff;
	strtab = file->map + offset_cigam_cpt;
	if (!check_memory(file, (void *)strtab))
		return (NULL);
	return (sym_parser_loop_32(file, lc, (void *)lst_sym_32, strtab));
}

static t_s_symbol			*sym_create_list_64(t_s_file *file,
											struct symtab_command *lc,
											char *strtab)
{
	struct nlist_64			*lst_sym_64;
	uint32_t				offset_cigam_cpt;

	offset_cigam_cpt = file->cigam ? swap_uint32(lc->symoff) : lc->symoff;
	lst_sym_64 = file->map + offset_cigam_cpt;
	if (!check_memory(file, (void *)lst_sym_64 + sizeof(struct nlist_64)
		* (file->cigam ? swap_uint32(lc->nsyms) : lc->nsyms)))
		return (NULL);
	offset_cigam_cpt = file->cigam ? swap_uint32(lc->stroff) : lc->stroff;
	strtab = file->map + offset_cigam_cpt;
	if (!check_memory(file, (void *)strtab))
		return (NULL);
	return (sym_parser_loop_64(file, lc, (void *)lst_sym_64, strtab));
}

t_s_symbol					*sym_parser(t_s_file *file,
										struct load_command *sym_com)
{
	t_s_symbol				*my_lst_sym;
	struct symtab_command	*lc_extended;

	lc_extended = (struct symtab_command *)sym_com;
	if (!check_formated_file(file, lc_extended))
	{
		ft_error(file->prog_name, "Malformated file.", file->name);
		return (NULL);
	}
	if (!check_memory(file, lc_extended + sizeof(struct symtab_command *)))
	{
		ft_error(file->prog_name, "Malformated file.", file->name);
		return (NULL);
	}
	if (file->format == MACHO_32BITS || file->format == FAT_32BITS)
		my_lst_sym = sym_create_list_32(file, lc_extended, NULL);
	else
		my_lst_sym = sym_create_list_64(file, lc_extended, NULL);
	if (my_lst_sym == NULL)
		ft_error(file->prog_name, "Malformated file.", file->name);
	return (my_lst_sym);
}
