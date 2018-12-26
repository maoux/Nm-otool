/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lc_parser.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heynard <heynard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/16 11:35:25 by heynard           #+#    #+#             */
/*   Updated: 2018/12/24 16:57:00 by heynard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

/*
** parse load commands and call sections parser to get sectons index
** 32 bits / 64 bits mach-o compatible
** little / big endian compatible
** error message self handled
*/

static int							lc_parser_init_macho(t_s_file *file,
												uint32_t *ncmd,
												struct load_command **p)
{
	struct mach_header				*hd_32;
	struct mach_header_64			*hd_64;

	if (file->format == MACHO_32BITS)
	{
		hd_32 = (struct mach_header *)file->map;
		if (!check_memory(file, (void *)hd_32 + sizeof(struct mach_header)))
			return (ft_error(file->prog_name, "Malformated file.", file->name));
		if ((file->ppc = (swap_uint32(hd_32->cputype) == CPU_TYPE_POWERPC)))
			file->cigam = 1;
		*p = (struct load_command *)(file->map + sizeof(struct mach_header));
		*ncmd = file->cigam ? swap_uint32(hd_32->ncmds) : hd_32->ncmds;
	}
	else if (file->format == MACHO_64BITS)
	{
		hd_64 = (struct mach_header_64 *)file->map;
		if (!check_memory(file, (void *)hd_64 + sizeof(struct mach_header_64)))
			return (ft_error(file->prog_name, "Malformated file.", file->name));
		if ((file->ppc = (swap_uint64(hd_64->cputype) == CPU_TYPE_POWERPC64)))
			file->cigam = 1;
		*p = (struct load_command *)(file->map + sizeof(struct mach_header_64));
		*ncmd = file->cigam ? swap_uint32(hd_64->ncmds) : hd_64->ncmds;
	}
	return (EXIT_SUCCESS);
}

static int							lc_parser_sect(t_s_file *file,
											uint32_t cmd_cigam_cpt,
											uint32_t *s_index,
											struct load_command *ptr)
{
	struct s_code_sgt				*sign;

	if (cmd_cigam_cpt == LC_SEGMENT)
	{
		if (sect_parser(file, (void *)ptr, s_index) == EXIT_FAILURE)
			return (EXIT_FAILURE);
	}
	else if (cmd_cigam_cpt == LC_SEGMENT_64)
	{
		if (sect_parser_64(file, (void *)ptr, s_index) == EXIT_FAILURE)
			return (EXIT_FAILURE);
	}
	if (cmd_cigam_cpt == LC_CODE_SIGNATURE)
	{
		sign = (struct s_code_sgt *)((void *)ptr + sizeof(struct load_command));
		if (!check_memory(file, (void *)sign + sizeof(struct s_code_sgt)))
			return (EXIT_FAILURE);
		file->end_of_signature = sign->data_off + sign->data_size;
	}
	return (EXIT_SUCCESS);
}

static struct load_command			*lc_parser_loop(t_s_file *file,
											uint32_t cmd,
											uint32_t ncmd,
											struct load_command *ptr)
{
	uint32_t						i;
	uint32_t						s_index;
	struct load_command				*save;
	uint32_t						cmd_cigam_cpt;

	i = -1;
	s_index = 0;
	save = NULL;
	if (ncmd > 1024)
		return (NULL);
	while (++i < ncmd)
	{
		cmd_cigam_cpt = file->cigam ? swap_uint32(ptr->cmd) : ptr->cmd;
		if (lc_parser_sect(file, cmd_cigam_cpt, &s_index, ptr) == EXIT_FAILURE)
			return (NULL);
		if (cmd_cigam_cpt == cmd)
			save = ptr;
		cmd_cigam_cpt = file->cigam ? swap_uint32(ptr->cmdsize) : ptr->cmdsize;
		ptr = (void *)ptr + cmd_cigam_cpt;
		if (!check_memory(file, (void *)ptr + sizeof(struct load_command)))
			return (NULL);
	}
	return (save);
}

struct load_command					*lc_parser(t_s_file *file, uint32_t cmd)
{
	struct load_command				*ptr;
	struct load_command				*save;
	uint32_t						ncmd;

	ptr = NULL;
	if (file->format == MACHO_32BITS || file->format == MACHO_64BITS)
	{
		if (lc_parser_init_macho(file, &ncmd, &ptr) == EXIT_FAILURE)
			return (NULL);
	}
	if (!check_memory(file, (void *)ptr + sizeof(struct load_command)))
	{
		ft_error(file->prog_name, "Malformated file.", file->name);
		return (NULL);
	}
	if ((save = lc_parser_loop(file, cmd, ncmd, ptr)) == NULL
		&& file->is_archive && file->prog_name != FT_OT_E)
		print_file_name(file, 2);
	else if (save == NULL && file->prog_name != FT_OT_E)
		ft_error(file->prog_name, "Malformated file.", file->name);
	return (save);
}
