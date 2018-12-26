/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm_file.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heynard <heynard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/13 20:54:47 by heynard           #+#    #+#             */
/*   Updated: 2018/12/23 21:38:19 by heynard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

static int				nm_handle_macho(t_s_file *file, const int nfiles)
{
	t_s_symbol			*lst;
	struct load_command *sym_com;

	lst = NULL;
	sym_com = NULL;
	if ((sym_com = lc_parser(file, LC_SYMTAB)))
		lst = sym_parser(file, sym_com);
	if (lst)
	{
		if (file->options & FLAG_u || file->options & FLAG_j)
			print_lst_sym_u(file, lst, nfiles);
		else
			print_lst_sym(file, lst, nfiles);
		free_lst_sym(lst);
		return (s_file_free(file, EXIT_SUCCESS));
	}
	else if (file->is_archive)
		return (s_file_free(file, EXIT_SUCCESS));
	return (s_file_free(file, EXIT_FAILURE));
}

static int				nm_handle_fat(t_s_file *file,
										const int nfiles,
										int ex_v,
										t_s_file *tmp)
{
	struct fat_header	*hdr;
	t_s_file			*arch_lst;

	hdr = (struct fat_header *)file->map;
	if (!check_memory(file, (void *)hdr + sizeof(struct fat_header *)))
	{
		s_file_free(file, EXIT_FAILURE);
		return (ft_error(file->prog_name, "Malformated file.", file->name));
	}
	file->narch = file->cigam ? swap_uint32(hdr->nfat_arch) : hdr->nfat_arch;
	arch_lst = fat_parse_arch(file, file->narch);
	if (!arch_lst)
		return (s_file_free(file, EXIT_FAILURE));
	tmp = arch_lst;
	while (tmp)
	{
		arch_lst = tmp->next;
		if ((ex_v = nm_handle_arch(tmp, nfiles)) == EXIT_FAILURE)
		{
			s_file_free_fat(arch_lst);
			return (s_file_free(file, ex_v));
		}
		tmp = arch_lst;
	}
	return (s_file_free(file, ex_v));
}

static int				nm_handle_archive(t_s_file *file, int ex_v, void *ptr)
{
	struct ar_hdr		*header;
	t_s_file			*obj_list;
	t_s_file			*tmp;

	header = (struct ar_hdr *)ptr;
	if (!check_memory(file, file->map + SARMAG + sizeof(struct ar_hdr)))
	{
		s_file_free(file, EXIT_FAILURE);
		return (ft_error(file->prog_name, "Malformated file.", file->name));
	}
	ptr += ft_atoi(header->ar_size) + sizeof(struct ar_hdr);
	if (!(obj_list = parse_archive(file, NULL, ptr)))
		return (s_file_free(file, EXIT_FAILURE));
	tmp = obj_list;
	while (tmp)
	{
		obj_list = tmp->next;
		if ((ex_v = nm_handle_arch(tmp, 2)) == EXIT_FAILURE)
		{
			s_file_free_fat(obj_list);
			return (s_file_free(file, ex_v));
		}
		tmp = obj_list;
	}
	return (s_file_free(file, ex_v));
}

int						nm_handle_arch(t_s_file *file, const int nfiles)
{
	if (file->format == MACHO_32BITS || file->format == MACHO_64BITS)
		return (nm_handle_macho(file, nfiles));
	if (file->format == FAT_32BITS || file->format == FAT_64BITS)
		return (nm_handle_fat(file, nfiles, EXIT_SUCCESS, NULL));
	if (file->format == AR)
		return (nm_handle_archive(file, EXIT_SUCCESS, file->map + SARMAG));
	return (EXIT_FAILURE);
}

int						nm_file(const char *file_name,
								const int options,
								const int nfiles)
{
	t_s_file			*file;
	int					code;

	file = NULL;
	code = s_file_get(file_name, &file, options);
	if (is_error(code))
	{
		ft_error(FT_NM_E, error_message_get(code), file_name);
		return (EXIT_FAILURE);
	}
	if (!file)
		return (EXIT_FAILURE);
	return (nm_handle_arch(file, nfiles));
}
