/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fat_parse_arch.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heynard <heynard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/19 18:14:18 by heynard           #+#    #+#             */
/*   Updated: 2018/12/23 19:02:54 by heynard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

static uint32_t			is_my_arch(t_s_file *file, void *arc, uint64_t *size)
{
	struct fat_arch		*f32;
	struct fat_arch_64	*f64;

	if (file->format == FAT_32BITS)
	{
		f32 = (struct fat_arch *)arc;
		if ((file->cigam ? swap_uint32(f32->cputype) : f32->cputype)
			== 16777223)
		{
			*size = file->cigam ? swap_uint32(f32->size) : f32->size;
			return (file->cigam ? swap_uint32(f32->offset) : f32->offset);
		}
	}
	else
	{
		f64 = (struct fat_arch_64 *)arc;
		if ((file->cigam ? swap_uint32(f64->cputype) : f64->cputype)
			== 16777223)
		{
			*size = file->cigam ? swap_uint64(f64->size) : f64->size;
			return (file->cigam ? swap_uint32(f64->offset) : f64->offset);
		}
	}
	return (0);
}

uint32_t				get_offset_myarch(t_s_file *file,
											void *arc,
											uint32_t narch,
											uint64_t *size)
{
	struct fat_arch		*f32;
	struct fat_arch_64	*f64;
	uint32_t			i;
	uint32_t			offset;

	i = -1;
	if (file->format == FAT_32BITS)
		f32 = (struct fat_arch *)arc;
	f64 = (struct fat_arch_64 *)arc;
	while (++i < narch)
	{
		if (file->format == FAT_32BITS)
		{
			if ((offset = is_my_arch(file, (void *)f32, size)) != 0)
				return (offset);
			f32 = (void *)f32 + sizeof(struct fat_arch);
		}
		else
		{
			if ((offset = is_my_arch(file, (void *)f64, size)) != 0)
				return (offset);
			f64 = (void *)f64 + sizeof(struct fat_arch_64);
		}
	}
	return (0);
}

t_s_file				*get_all_arch(t_s_file *file,
										void *arc,
										uint32_t narch,
										t_s_file *arch_lst)
{
	struct fat_arch		*f32;
	struct fat_arch_64	*f64;
	uint32_t			i;

	i = -1;
	if (file->format == FAT_32BITS)
		f32 = (struct fat_arch *)arc;
	else
		f64 = (struct fat_arch_64 *)arc;
	while (++i < narch)
	{
		if (file->format == FAT_32BITS)
		{
			if ((arch_lst = add_arch_fat32(file, f32, arch_lst)) == NULL)
				break ;
			f32 = (void *)f32 + sizeof(struct fat_arch);
		}
		else
		{
			if ((arch_lst = add_arch_fat64(file, f64, arch_lst)) == NULL)
				break ;
			f64 = (void *)f64 + sizeof(struct fat_arch);
		}
	}
	return (arch_lst);
}

t_s_file				*fat_parse_loop(t_s_file *file,
										void *arc,
										uint32_t narch)
{
	t_s_file			*arch_lst;
	uint32_t			offset;
	uint64_t			file_sub_size;
	int					code;

	arch_lst = NULL;
	if ((offset = get_offset_myarch(file, arc, narch, &file_sub_size)) != 0)
	{
		code = s_file_get_fat(&arch_lst, file->options, file->map + offset);
		if (arch_lst)
			code = s_file_up(file->name, arch_lst, file_sub_size, file->size);
		arch_lst->narch = 0;
		if (is_error(code))
		{
			ft_error(FT_NM_E, error_message_get(code), file->name);
			return (NULL);
		}
		return (arch_lst);
	}
	else
		return (get_all_arch(file, arc, narch, arch_lst));
	return (NULL);
}

t_s_file				*fat_parse_arch(t_s_file *file, uint32_t narch)
{
	struct fat_arch		*f32;
	struct fat_arch_64	*f64;

	if (file->format == FAT_32BITS)
	{
		f32 = (struct fat_arch *)(file->map + sizeof(struct fat_header));
		if (!check_memory(file, (void *)f32 + sizeof(struct fat_arch) * narch))
		{
			ft_error(file->prog_name, "Malformated file.", file->name);
			return (NULL);
		}
	}
	else
	{
		f64 = (struct fat_arch_64 *)(file->map + sizeof(struct fat_header));
		if (!check_memory(file, (void *)f64
			+ sizeof(struct fat_arch_64) * narch))
		{
			ft_error(file->prog_name, "Malformated file.", file->name);
			return (NULL);
		}
	}
	if (file->format == FAT_32BITS)
		return (fat_parse_loop(file, (void *)f32, narch));
	return (fat_parse_loop(file, (void *)f64, narch));
}
