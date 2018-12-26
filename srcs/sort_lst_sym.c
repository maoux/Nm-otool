/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_lst_sym.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heynard <heynard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/21 10:31:40 by heynard           #+#    #+#             */
/*   Updated: 2018/12/24 14:59:04 by heynard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

t_s_symbol				*sort_alnum_rev(t_s_symbol *new, t_s_symbol *lst)
{
	t_s_symbol			*prev;
	t_s_symbol			*first;

	first = lst;
	if (ft_strcmp(new->name, lst->name) > 0)
	{
		new->next = lst;
		return (new);
	}
	while (lst->next)
	{
		prev = lst;
		lst = lst->next;
		if (ft_strcmp(new->name, lst->name) > 0)
		{
			prev->next = new;
			new->next = lst;
			return (first);
		}
	}
	lst->next = new;
	return (first);
}

int						ft_cmpval(t_s_file *file,
								t_s_symbol *new,
								t_s_symbol *lst)
{
	if (file->format == MACHO_32BITS || file->format == FAT_32BITS)
	{
		if (new->value.val_32 < lst->value.val_32)
			return (TRUE);
		else
			return (FALSE);
	}
	else
	{
		if (new->value.val_64 < lst->value.val_64)
			return (TRUE);
		else
			return (FALSE);
	}
	return (FALSE);
}

t_s_symbol				*sort_alnum_loop(t_s_file *file,
									t_s_symbol *new,
									t_s_symbol *lst)
{
	t_s_symbol			*prev;
	t_s_symbol			*first;

	first = lst;
	while (lst->next)
	{
		prev = lst;
		lst = lst->next;
		if (ft_strcmp(new->name, lst->name) < 0)
		{
			prev->next = new;
			new->next = lst;
			return (first);
		}
		else if (ft_strcmp(new->name, lst->name) == 0
				&& ft_cmpval(file, new, lst))
		{
			prev->next = new;
			new->next = lst;
			return (first);
		}
	}
	lst->next = new;
	return (first);
}

t_s_symbol				*sort_alnum(t_s_file *file,
									t_s_symbol *new,
									t_s_symbol *lst)
{
	if (ft_strcmp(new->name, lst->name) < 0)
	{
		new->next = lst;
		return (new);
	}
	else if (ft_strcmp(new->name, lst->name) == 0 && ft_cmpval(file, new, lst))
	{
		new->next = lst;
		return (new);
	}
	return (sort_alnum_loop(file, new, lst));
}

t_s_symbol				*lst_unsort(t_s_symbol *new, t_s_symbol *lst)
{
	t_s_symbol			*tmp;

	tmp = lst;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
	return (lst);
}
