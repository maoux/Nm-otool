/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   archive_parser.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heynard <heynard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/23 14:57:40 by heynard           #+#    #+#             */
/*   Updated: 2018/12/24 18:15:31 by heynard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

static t_s_file		*add_archive_obj(t_s_file *new, t_s_file *obj_lst)
{
	t_s_file		*first;

	if (!obj_lst)
		return (new);
	first = obj_lst;
	while (obj_lst->next)
		obj_lst = obj_lst->next;
	obj_lst->next = new;
	return (first);
}

static t_s_file		*create_archive_obj(t_s_file *file,
									void *ptr,
									char *str,
									uint32_t size)
{
	t_s_file		*new;
	uint32_t		code;

	new = NULL;
	code = s_file_get_fat(&new, file->options, ptr);
	if (new)
		code = s_file_up(file->name, new, size, file->size);
	if (is_error(code))
	{
		ft_error(FT_NM_E, error_message_get(code), file->name);
		return (NULL);
	}
	if ((new->obj_name = ft_strdup(str)) == NULL)
	{
		ft_error(FT_NM_E, error_message_get(code), file->name);
		return (NULL);
	}
	new->prog_name = file->prog_name;
	new->is_archive = 1;
	return (new);
}

t_s_file			*parse_archive(t_s_file *file, t_s_file *obj_lst, void *ptr)
{
	t_s_file		*new;
	struct ar_hdr	*header;
	uint32_t		len;
	char			*str;

	while (ptr)
	{
		header = (struct ar_hdr *)ptr;
		if (!check_memory(file, ptr + sizeof(struct ar_hdr)))
			return (obj_lst);
		str = ptr + sizeof(struct ar_hdr);
		len = ft_strlen(str);
		while (!str[len++])
			;
		new = create_archive_obj(file, ptr + sizeof(struct ar_hdr) + len - 1,
								str, ft_atoi(header->ar_size));
		if (!new)
		{
			s_file_free_fat(obj_lst);
			return (NULL);
		}
		obj_lst = add_archive_obj(new, obj_lst);
		ptr += ft_atoi(header->ar_size) + sizeof(struct ar_hdr);
	}
	return (obj_lst);
}
