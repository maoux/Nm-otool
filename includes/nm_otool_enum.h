/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm_otool_enum.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heynard <heynard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/13 10:33:39 by heynard           #+#    #+#             */
/*   Updated: 2018/12/23 21:44:40 by heynard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NM_OTOOL_ENUM_H
# define NM_OTOOL_ENUM_H

/*
** used to know which program the error function is running in
*/
enum			e_error
{
	FT_NM_E = -1,
	FT_OT_E = -2,
	TYPE_DIR_E = -3,
	TYPE_E = -4,
	OPEN_E = -5,
	MMAP_E = -6,
	FORMAT_E = -7,
	MALLOC_E = -8,
	FSTAT_E = -9
};

/*
** checklist : u U r p j
*/

enum			e_option
{
	FLAG_u = 1 << 0,
	FLAG_U = 1 << 1,
	FLAG_r = 1 << 3,
	FLAG_p = 1 << 4,
	FLAG_j = 1 << 5
};

/*
** state values verbose
** useful to parse program arguments
*/
enum			e_state
{
	STOP_OPTION,
	IS_OPTION,
	TWO_OCCUR_E,
	UNDEFINED_E,
	DEFAULT
};

enum			e_file_type
{
	MACHO_32BITS,
	MACHO_64BITS,
	FAT_32BITS,
	FAT_64BITS,
	AR,
	OTHER,
	NO_PTR_ERROR
};

typedef enum e_file_type	t_file_type;

#endif
