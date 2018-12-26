/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm_otool_struct.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heynard <heynard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/13 20:46:55 by heynard           #+#    #+#             */
/*   Updated: 2018/12/24 17:33:23 by heynard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NM_OTOOL_STRUCT_H
# define NM_OTOOL_STRUCT_H

union			u_value
{
	uint32_t	val_32;
	uint64_t	val_64;
};

struct			s_file
{
	char				*name;
	char				*obj_name;
	int					fd;
	void				*map;
	uint64_t			size;
	char				cigam;
	t_file_type			format;
	int					prog_name;
	int					options;
	uint8_t				is_full_file;
	uint8_t				is_archive;
	struct section		**sections_32;
	struct section_64	**sections_64;
	uint32_t			text_nsect;
	uint32_t			data_nsect;
	uint32_t			bss_nsect;
	union u_value		size_text_sect;
	union u_value		addr_text_sect;
	uint32_t			offset_text_sect;
	uint32_t			narch;
	uint32_t			ppc;
	uint64_t			end_of_signature;
	struct s_file		*next;
};
struct			s_symbol
{
	uint8_t			type;
	uint8_t			sect;
	char			*name;
	union u_value	value;
	struct s_symbol	*next;
};

/*
** code signature
** lc_parser.c section header parsing
*/

struct			s_code_sgt
{
	uint32_t	data_off;
	uint32_t	data_size;
};

typedef struct s_file		t_s_file;
typedef struct s_symbol		t_s_symbol;

#endif
