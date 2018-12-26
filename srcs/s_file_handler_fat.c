/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   s_file_handler_fat.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heynard <heynard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/20 16:11:13 by heynard           #+#    #+#             */
/*   Updated: 2018/12/23 12:15:07 by heynard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

/*
** only in error cases
** see nm_file.c
*/

void				s_file_free_fat(t_s_file *arch_lst)
{
	t_s_file		*tmp;

	if (arch_lst)
	{
		tmp = arch_lst;
		while (tmp)
		{
			tmp = arch_lst->next;
			s_file_free(arch_lst, EXIT_FAILURE);
			arch_lst = tmp;
		}
	}
}

int					s_file_up(const char *file_name,
								t_s_file *file,
								uint64_t size,
								uint64_t fat_size)
{
	if (size >= fat_size)
		return (s_file_free(file, FORMAT_E));
	file->size = size;
	file->is_full_file = FALSE;
	s_file_get_format(file);
	if (file->format == OTHER)
		return (s_file_free(file, FORMAT_E));
	if ((file->name = ft_strdup(file_name)) == NULL)
		return (s_file_free(file, MALLOC_E));
	return (EXIT_SUCCESS);
}

int					s_file_get_fat(t_s_file **f,
									int opt,
									void *map)
{
	t_s_file		*file;

	if ((file = (t_s_file *)malloc(sizeof(t_s_file))) == NULL)
		return (MALLOC_E);
	nm_init_file(file, opt);
	file->map = map;
	*f = file;
	return (EXIT_SUCCESS);
}
