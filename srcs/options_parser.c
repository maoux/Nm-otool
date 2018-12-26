/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   options_parser.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heynard <heynard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/13 17:29:15 by heynard           #+#    #+#             */
/*   Updated: 2018/12/24 13:55:33 by heynard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

static int		set_option(int state, char c, int *options)
{
	if (c == 'u' && !(*options & FLAG_u))
		*options |= FLAG_u;
	else if (c == 'U' && !(*options & FLAG_U))
		*options |= FLAG_U;
	else if (c == 'j' && !(*options & FLAG_j))
		*options |= FLAG_j;
	else if (c == 'r' && !(*options & FLAG_r))
		*options |= FLAG_r;
	else if (c == 'p' && !(*options & FLAG_p))
		*options |= FLAG_p;
	else
		return (state = TWO_OCCUR_E);
	return (state);
}

static int		parse_param(char *str, int *options)
{
	int			state;
	int			i;

	state = DEFAULT;
	i = -1;
	while (str[++i])
	{
		if (i == 0 && str[i] == '-')
			state = IS_OPTION;
		else if (state == IS_OPTION && i == 1
				&& str[i] == '-' && str[i + 1] == '\0')
			return (STOP_OPTION);
		else if (state == IS_OPTION && ft_strchr("uUjpr", str[i]))
		{
			if ((state = set_option(state, str[i], options)) == TWO_OCCUR_E)
				return (state);
		}
		else if (state == IS_OPTION)
			return (UNDEFINED_E);
		else
			return (DEFAULT);
	}
	if (i == 1 && state == IS_OPTION)
		return (DEFAULT);
	return (state);
}

int				parse_options(int ac, char **av,
							int *options, int **opt_positions)
{
	int			i;
	int			state;

	i = 0;
	while (++i < ac)
	{
		state = parse_param(av[i], options);
		if (state == IS_OPTION || state == STOP_OPTION)
			(*opt_positions)[i] = 1;
		else
			(*opt_positions)[i] = 0;
		if (state == STOP_OPTION)
			break ;
		if (state == TWO_OCCUR_E)
			return (ft_error(FT_NM_E, "can't use same option 2 times", NULL));
		if (state == UNDEFINED_E)
			return (ft_error(FT_NM_E, "undefined paramater", NULL));
	}
	return (EXIT_SUCCESS);
}
