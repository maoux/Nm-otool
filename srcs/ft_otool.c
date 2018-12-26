/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_otool.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heynard <heynard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/13 17:28:00 by heynard           #+#    #+#             */
/*   Updated: 2018/12/22 18:31:16 by heynard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

static int			ot_print_usage(void)
{
	ft_printf("ft_otool: usage: otool directory/ft_otool file1 [file2 ...]\n");
	return (EXIT_FAILURE);
}

int					main(int ac, char **av)
{
	int				i;

	if (ac <= 1)
		return (ot_print_usage());
	i = 1;
	while (i < ac)
	{
		if (otool_file(av[i]) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		i++;
	}
	return (EXIT_SUCCESS);
}
