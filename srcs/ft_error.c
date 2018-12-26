/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_error.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heynard <heynard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/21 13:01:25 by heynard           #+#    #+#             */
/*   Updated: 2018/12/23 18:50:26 by heynard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"
#include "nm_otool_enum.h"

char			*error_message_get(int code)
{
	static char	*(msg[10]) = {
		"default",
		"default",
		"default",
		"Is a directory.",
		"The file was not recognized as a valid object file.\n",
		"No such file or directory.",
		"Mmap failed.",
		"The file was not recognized as a valid object file.\n",
		"Malloc failed.",
		"Fstat failed."
	};

	if (code == OPEN_E && errno == EACCES)
		return ("Permission denied.");
	if (code > 9)
		return ("");
	return (msg[-code]);
}

int				is_error(int code)
{
	if (code == TYPE_DIR_E || code == TYPE_E || code == OPEN_E
		|| code == FSTAT_E || code == MMAP_E || code == FORMAT_E
		|| code == MALLOC_E)
		return (TRUE);
	return (FALSE);
}

/*
** args :		programme name, error message and file name to display
** functions :	display the program name, file name and error message in
**				this specific order on STDERR
**				if file_name is NULL, just ignore it
** return		EXIT_FAILURE value for cascade use
*/

int				ft_error(int prog, char *msg, const char *file_name)
{
	if (prog == FT_NM_E)
		ft_dprintf(STDERR_FILENO, "ft_nm: ");
	else if (prog == FT_OT_E)
		ft_dprintf(STDERR_FILENO, "ft_otool: ");
	if (file_name)
		ft_dprintf(STDERR_FILENO, "%s: ", file_name);
	if (msg)
		ft_dprintf(STDERR_FILENO, "%s\n", msg);
	else
		ft_dprintf(STDERR_FILENO, "an error occured.\n");
	return (EXIT_FAILURE);
}
