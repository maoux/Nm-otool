/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fat_add_lst_arch.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heynard <heynard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/20 20:27:08 by heynard           #+#    #+#             */
/*   Updated: 2018/12/23 12:17:44 by heynard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

t_s_file			*add_arch_fat64(t_s_file *file,
									struct fat_arch_64 *f64,
									t_s_file *arch_lst)
{
	t_s_file		*new;
	t_s_file		*first;
	uint32_t		offset;
	int				code;

	new = NULL;
	offset = file->cigam ? swap_uint32(f64->offset) : f64->offset;
	code = s_file_get_fat(&new, file->options, file->map + offset);
	if (new)
		code = s_file_up(file->name, new,
			file->cigam ? swap_uint64(f64->size) : f64->size, file->size);
	new->narch = file->narch;
	if (is_error(code))
	{
		ft_error(FT_NM_E, error_message_get(code), file->name);
		s_file_free_fat(arch_lst);
		return (NULL);
	}
	if (!arch_lst)
		return (new);
	first = arch_lst;
	while (arch_lst->next)
		arch_lst = arch_lst->next;
	arch_lst->next = new;
	return (first);
}

t_s_file			*add_arch_fat32(t_s_file *file,
									struct fat_arch *f32,
									t_s_file *arch_lst)
{
	t_s_file		*new;
	t_s_file		*first;
	uint32_t		offset;
	int				code;

	new = NULL;
	offset = file->cigam ? swap_uint32(f32->offset) : f32->offset;
	code = s_file_get_fat(&new, file->options, file->map + offset);
	if (new)
		code = s_file_up(file->name, new,
				file->cigam ? swap_uint32(f32->size) : f32->size, file->size);
	new->narch = file->narch;
	if (is_error(code))
	{
		ft_error(FT_NM_E, error_message_get(code), file->name);
		s_file_free_fat(arch_lst);
		return (NULL);
	}
	if (!arch_lst)
		return (new);
	first = arch_lst;
	while (arch_lst->next)
		arch_lst = arch_lst->next;
	arch_lst->next = new;
	return (first);
}
