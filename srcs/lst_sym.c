/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_sym.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heynard <heynard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/16 18:50:00 by heynard           #+#    #+#             */
/*   Updated: 2018/12/24 18:05:00 by heynard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

static t_s_symbol		*init_lst_sym(t_s_file *file,
									t_s_symbol *new,
									char *sym_name,
									void *lst_sym)
{
	struct nlist_64			*lst_sym_64;
	struct nlist			*lst_sym_32;

	if (file->format == MACHO_32BITS)
	{
		lst_sym_32 = (struct nlist *)lst_sym;
		new->type = lst_sym_32->n_type;
		new->value.val_32 = lst_sym_32->n_value;
		new->sect = lst_sym_32->n_sect;
	}
	else
	{
		lst_sym_64 = (struct nlist_64 *)lst_sym;
		new->type = lst_sym_64->n_type;
		new->value.val_64 = lst_sym_64->n_value;
		new->sect = lst_sym_64->n_sect;
	}
	if (sym_name)
		new->name = ft_strdup(sym_name);
	else
		new->name = NULL;
	new->next = NULL;
	return (new);
}

void					free_lst_sym(t_s_symbol *lst)
{
	t_s_symbol			*tmp;

	while (lst)
	{
		tmp = lst;
		lst = lst->next;
		ft_strdel(&tmp->name);
		free(tmp);
	}
}

t_s_symbol				*add_lst_sym(t_s_file *file,
									t_s_symbol *lst,
									char *sym_name,
									void *lst_sym)
{
	t_s_symbol			*new;

	if (!ft_strcmp("radr://5614542", sym_name))
		return (lst);
	if (!(new = (t_s_symbol *)malloc(sizeof(t_s_symbol))))
		return (lst);
	new = init_lst_sym(file, new, sym_name, lst_sym);
	if (!lst)
		return (new);
	if (file->options & FLAG_p)
		return (lst_unsort(new, lst));
	if (file->options & FLAG_r)
		return (sort_alnum_rev(new, lst));
	return (sort_alnum(file, new, lst));
}
