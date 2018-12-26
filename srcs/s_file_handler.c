/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   s_file_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heynard <heynard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/13 21:00:31 by heynard           #+#    #+#             */
/*   Updated: 2018/12/24 16:27:24 by heynard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

/*
** open fd
** map file into memory, get some infos about the file
** free file structure and close file
** mach-o 32 / 64 compatible
** little / big endian compatible
*/

void				nm_init_file(t_s_file *file, int options)
{
	if (file)
	{
		file->prog_name = FT_NM_E;
		file->options = options;
		file->bss_nsect = NO_SECT;
		file->text_nsect = NO_SECT;
		file->bss_nsect = NO_SECT;
		file->format = OTHER;
		file->is_archive = 0;
		file->end_of_signature = 0;
		file->size = 0;
		file->narch = 1;
		file->ppc = 0;
		file->sections_32 = NULL;
		file->sections_64 = NULL;
		file->name = NULL;
		file->obj_name = NULL;
		file->map = NULL;
		file->next = NULL;
		file->offset_text_sect = 0;
		file->size_text_sect.val_32 = 0;
		file->size_text_sect.val_64 = 0;
		file->addr_text_sect.val_32 = 0;
		file->addr_text_sect.val_64 = 0;
	}
}

int					s_file_stat(t_s_file *file)
{
	struct stat		buf;

	if (fstat(file->fd, &buf) < 0)
		return (FSTAT_E);
	if (!S_ISREG(buf.st_mode))
	{
		if (S_ISDIR(buf.st_mode))
			return (TYPE_DIR_E);
		return (TYPE_E);
	}
	file->size = buf.st_size;
	return (0);
}

void				s_file_get_format(t_s_file *file)
{
	uint32_t		magic;

	if (!check_memory(file, file->map + sizeof(uint64_t)))
		return ;
	magic = *(uint32_t *)file->map;
	if (magic == (uint32_t)MH_MAGIC || magic == (uint32_t)MH_CIGAM)
		file->format = MACHO_32BITS;
	else if (magic == (uint32_t)MH_MAGIC_64 || magic == (uint32_t)MH_CIGAM_64)
		file->format = MACHO_64BITS;
	else if (magic == (uint32_t)FAT_MAGIC || magic == (uint32_t)FAT_CIGAM)
		file->format = FAT_32BITS;
	else if (magic == (uint32_t)FAT_MAGIC_64 || magic == (uint32_t)FAT_CIGAM_64)
		file->format = FAT_64BITS;
	else if ((uint64_t)magic == AR_MAGIC
			|| magic == (uint32_t)AR_CIGAM)
		file->format = AR;
	else
		return ;
	if (magic == (uint32_t)MH_CIGAM || magic == (uint32_t)MH_CIGAM
	|| magic == (uint32_t)FAT_CIGAM || magic == (uint32_t)FAT_CIGAM_64
	|| magic == (uint32_t)AR_CIGAM)
		file->cigam = TRUE;
	else
		file->cigam = FALSE;
}

int					s_file_free(t_s_file *file, int exit_value)
{
	if (file)
	{
		if (file->fd >= 0 && file->is_full_file == TRUE)
		{
			if (close(file->fd) < 0)
				exit_value = EXIT_FAILURE;
		}
		if (file->map && file->is_full_file == TRUE)
		{
			if (munmap(file->map, file->size) < 0)
				exit_value = EXIT_FAILURE;
		}
		if (file->name)
			ft_strdel(&file->name);
		if (file->obj_name)
			ft_strdel(&file->obj_name);
		if (file->sections_32)
			free(file->sections_32);
		if (file->sections_64)
			free(file->sections_64);
		free(file);
	}
	return (exit_value);
}

int					s_file_get(const char *file_name, t_s_file **f, int opt)
{
	t_s_file		*file;
	int				error_type;

	if ((file = (t_s_file *)malloc(sizeof(t_s_file))) == NULL)
		return (MALLOC_E);
	nm_init_file(file, opt);
	file->is_full_file = TRUE;
	if ((file->fd = open(file_name, O_RDONLY)) < 0)
		return (s_file_free(file, OPEN_E));
	if ((error_type = s_file_stat(file)) < 0)
		return (s_file_free(file, error_type));
	if ((file->map = mmap(0, file->size,
	PROT_READ, MAP_PRIVATE, file->fd, 0)) == MAP_FAILED)
		return (s_file_free(file, MMAP_E));
	s_file_get_format(file);
	if (file->format == OTHER)
		return (s_file_free(file, FORMAT_E));
	if ((file->name = ft_strdup(file_name)) == NULL)
		return (s_file_free(file, MALLOC_E));
	*f = file;
	return (EXIT_SUCCESS);
}
