/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sect_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heynard <heynard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/18 17:22:44 by heynard           #+#    #+#             */
/*   Updated: 2018/12/24 16:25:28 by heynard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

/*
** parse sections header and set sections index for further use with
** symbol table structure
** not self handling errors
** little / big endian compatible
** mach-o 32 / 64 bits compatible
*/

static void						sect_parser_cmp(t_s_file *file,
												struct section *sect,
												uint32_t i,
												uint32_t s_index)
{
	if (ft_strcmp((sect + i)->sectname, SECT_TEXT) == 0
		&& ft_strcmp((sect + i)->segname, SEG_TEXT) == 0)
	{
		file->text_nsect = s_index + 1;
		file->addr_text_sect.val_32 = file->cigam ?
			swap_uint32((sect + i)->addr) : (sect + i)->addr;
		file->offset_text_sect = file->cigam ?
			swap_uint32((sect + i)->offset) : (sect + i)->offset;
		file->size_text_sect.val_32 = file->cigam ?
			swap_uint32((sect + i)->size) : (sect + i)->size;
	}
	else if (ft_strcmp((sect + i)->sectname, SECT_DATA) == 0
		&& ft_strcmp((sect + i)->segname, SEG_DATA) == 0)
		file->data_nsect = s_index + 1;
	else if (ft_strcmp((sect + i)->sectname, SECT_BSS) == 0
		&& ft_strcmp((sect + i)->segname, SEG_DATA) == 0)
		file->bss_nsect = s_index + 1;
}

static void						sect_parser_64_cmp(t_s_file *file,
												struct section_64 *sect,
												uint32_t i,
												uint32_t s_index)
{
	if (ft_strcmp((sect + i)->sectname, SECT_TEXT) == 0
		&& ft_strcmp((sect + i)->segname, SEG_TEXT) == 0)
	{
		file->text_nsect = s_index + 1;
		file->addr_text_sect.val_64 = file->cigam ?
			swap_uint64((sect + i)->addr) : (sect + i)->addr;
		file->offset_text_sect = file->cigam ?
			swap_uint32((sect + i)->offset) : (sect + i)->offset;
		file->size_text_sect.val_64 = file->cigam ?
			swap_uint64((sect + i)->size) : (sect + i)->size;
	}
	else if (ft_strcmp((sect + i)->sectname, SECT_DATA) == 0
		&& ft_strcmp((sect + i)->segname, SEG_DATA) == 0)
		file->data_nsect = s_index + 1;
	else if (ft_strcmp((sect + i)->sectname, SECT_BSS) == 0
		&& ft_strcmp((sect + i)->segname, SEG_DATA) == 0)
		file->bss_nsect = s_index + 1;
}

int								sect_parser(t_s_file *file, void *lc,
											uint32_t *s_index)
{
	struct segment_command		*sg;
	struct section				*sect;
	uint32_t					i;
	uint32_t					nsects;

	i = 0;
	sg = (struct segment_command *)lc;
	sect = (struct section *)((char *)sg + sizeof(struct segment_command));
	if (check_memory(file, (void *)sect + sizeof(struct section)) == FALSE)
		return (EXIT_FAILURE);
	nsects = file->cigam ? swap_uint32(sg->nsects) : sg->nsects;
	while (i < nsects)
	{
		sect_parser_cmp(file, sect, i, *s_index);
		(*s_index)++;
		i++;
	}
	return (EXIT_SUCCESS);
}

int								sect_parser_64(t_s_file *file, void *lc,
												uint32_t *s_index)
{
	struct segment_command_64	*sg;
	struct section_64			*st;
	uint32_t					i;
	uint32_t					nsects;

	i = 0;
	sg = (struct segment_command_64 *)lc;
	st = (struct section_64 *)((char *)sg + sizeof(struct segment_command_64));
	if (!check_memory(file, (void *)st + sizeof(struct section_64)))
		return (EXIT_FAILURE);
	nsects = file->cigam ? swap_uint32(sg->nsects) : sg->nsects;
	while (i < nsects)
	{
		sect_parser_64_cmp(file, st, i, *s_index);
		(*s_index)++;
		i++;
	}
	return (EXIT_SUCCESS);
}
