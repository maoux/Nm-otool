/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heynard <heynard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/20 22:20:15 by heynard           #+#    #+#             */
/*   Updated: 2018/12/23 18:29:12 by heynard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"
#include "nm_otool_enum.h"

/*
** files to test :
**
** little endian / fat32 -> /usr/lib/libkrb4.dylib
*/

static void		zero_positions(int ac, int **opt_positions)
{
	int			i;

	i = 0;
	while (i < ac)
	{
		(*opt_positions)[i] = 0;
		i++;
	}
}

static int		get_nb_files(int *opt_positions, int ac)
{
	int			nb;
	int			i;

	i = 1;
	nb = 0;
	while (i < ac)
	{
		if (!opt_positions[i])
			nb++;
		i++;
	}
	return (nb);
}

static int		nm_each_files(int ac, char **av, int opt, int *opt_positions)
{
	int			exit_value;
	int			i;
	int			nfiles;

	i = 0;
	if (ac < 1)
		return (EXIT_FAILURE);
	if ((nfiles = get_nb_files(opt_positions, ac)) == 0)
		return (nm_file("a.out", opt, 1));
	while (++i < ac)
	{
		if (!opt_positions[i])
		{
			if (exit_value == EXIT_FAILURE)
				nm_file(av[i], opt, nfiles);
			else
				exit_value = nm_file(av[i], opt, nfiles);
		}
	}
	return (exit_value);
}

int				main(int ac, char **av)
{
	int			exit_value;
	int			options;
	int			*opt_positions;

	exit_value = EXIT_SUCCESS;
	options = 0;
	if (ac < 2)
		return (nm_file("a.out", options, 1));
	else
	{
		if (!(opt_positions = (int *)malloc(sizeof(int) * (ac + 1))))
			return (EXIT_FAILURE);
		zero_positions(ac, &opt_positions);
		if (parse_options(ac, av, &options, &opt_positions) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		exit_value = nm_each_files(ac, av, options, opt_positions);
		free(opt_positions);
	}
	return (exit_value);
}
