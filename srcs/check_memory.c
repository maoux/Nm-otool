/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_memory.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heynard <heynard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/13 21:51:05 by heynard           #+#    #+#             */
/*   Updated: 2018/12/23 17:54:51 by heynard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool_enum.h"
#include "nm_otool.h"

int				check_formated_file(t_s_file *file, struct symtab_command *lc)
{
	uint32_t	stroff;
	uint32_t	strsize;

	if (file && lc)
	{
		if (file->is_archive)
			return (TRUE);
		stroff = file->cigam ? swap_uint32(lc->stroff) : lc->stroff;
		strsize = file->cigam ? swap_uint32(lc->strsize) : lc->strsize;
		if (file->end_of_signature != 0 && file->end_of_signature == file->size)
			return (TRUE);
		if (stroff + strsize != file->size)
			return (FALSE);
	}
	return (TRUE);
}

int				check_memory(t_s_file *file, void *ptr)
{
	if (file && ptr)
	{
		if (file->is_archive)
			return (TRUE);
		if (ptr > (void *)(file->map + file->size))
			return (FALSE);
		return (TRUE);
	}
	return (FALSE);
}
