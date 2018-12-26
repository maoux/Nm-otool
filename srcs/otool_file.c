/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   otool_file.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heynard <heynard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/22 18:32:22 by heynard           #+#    #+#             */
/*   Updated: 2018/12/24 17:04:11 by heynard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

static int				ot_handle_macho(t_s_file *file,
										t_s_symbol *lst,
										struct load_command *sym_com)
{
	file->prog_name = FT_OT_E;
	if ((sym_com = lc_parser(file, LC_SYMTAB)))
		lst = sym_parser(file, sym_com);
	if (lst)
	{
		if (!check_memory(file, file->map
			+ file->offset_text_sect + (file->format == MACHO_32BITS
			? file->size_text_sect.val_32 : file->size_text_sect.val_64)))
		{
			ft_error(file->prog_name, "is not an object file", file->name);
		}
		else
			print_section_text(file);
		free_lst_sym(lst);
		return (s_file_free(file, EXIT_SUCCESS));
	}
	else if (file->is_archive)
	{
		print_section_text(file);
		return (s_file_free(file, EXIT_SUCCESS));
	}
	return (s_file_free(file, EXIT_FAILURE));
}

static int				ot_handle_fat(t_s_file *file,
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
		if ((ex_v = ot_handle_arch(tmp)) == EXIT_FAILURE)
		{
			s_file_free_fat(arch_lst);
			return (s_file_free(file, ex_v));
		}
		tmp = arch_lst;
	}
	return (s_file_free(file, ex_v));
}

static int				ot_handle_archive(t_s_file *file,
										int ex_v,
										void *ptr,
										t_s_file *obj_list)
{
	struct ar_hdr		*header;
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
	ft_printf("Archive : %s\n", file->name);
	while (tmp)
	{
		obj_list = tmp->next;
		if ((ex_v = ot_handle_arch(tmp)) == EXIT_FAILURE)
		{
			s_file_free_fat(obj_list);
			return (s_file_free(file, ex_v));
		}
		tmp = obj_list;
	}
	return (s_file_free(file, ex_v));
}

int						ot_handle_arch(t_s_file *file)
{
	if (file->format == MACHO_32BITS || file->format == MACHO_64BITS)
		return (ot_handle_macho(file, NULL, NULL));
	if (file->format == FAT_32BITS || file->format == FAT_64BITS)
		return (ot_handle_fat(file, EXIT_SUCCESS, NULL));
	if (file->format == AR)
		return (ot_handle_archive(file, 0, file->map + SARMAG, NULL));
	return (EXIT_FAILURE);
}

int						otool_file(const char *file_name)
{
	t_s_file			*file;
	int					code;

	file = NULL;
	code = s_file_get(file_name, &file, 0);
	if (is_error(code))
	{
		ft_error(FT_OT_E, error_message_get(code), file_name);
		return (EXIT_FAILURE);
	}
	if (!file)
		return (EXIT_FAILURE);
	file->prog_name = FT_OT_E;
	return (ot_handle_arch(file));
}
