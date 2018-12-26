/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm_otool.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heynard <heynard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/20 21:51:47 by heynard           #+#    #+#             */
/*   Updated: 2018/12/24 18:41:45 by heynard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NM_OTOOL_H
# define NM_OTOOL_H

# include "../libft/includes/libft.h"
# include <fcntl.h>
# include <sys/mman.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <mach-o/nlist.h>
# include <mach-o/loader.h>
# include <mach-o/fat.h>
# include <mach-o/ranlib.h>
# include <errno.h>
# include <ar.h>

# include "nm_otool_enum.h"
# include "nm_otool_struct.h"

# define AR_MAGIC	0x213c617263683e0a
# define AR_CIGAM	0x0a3e686372613c21

# define A_U		0b0000000000000001

# define TRUE		1
# define FALSE		0

int					ft_error(int prog, char *msg, const char *file_name);
char				*error_message_get(int code);
int					is_error(int code);
int					check_memory(t_s_file *file, void *ptr);
int					check_formated_file(t_s_file *file,
							struct symtab_command *lc);

int					parse_options(int ac, char **av,
							int *options, int **opt_positions);
int					nm_file(const char *file_name, const int options,
							const int nfiles);
int					otool_file(const char *file_name);
int					ot_handle_arch(t_s_file *file);
int					nm_handle_arch(t_s_file *file, const int nfiles);

int					s_file_get(const char *file_name, t_s_file **f, int opt);
int					s_file_free(t_s_file *file, int return_value);
void				s_file_get_format(t_s_file *file);
int					s_file_stat(t_s_file *file);
void				nm_init_file(t_s_file *file, int options);
int					s_file_get_fat(t_s_file **f,
							int opt,
							void *map);
int					s_file_up(const char *file_name,
							t_s_file *file,
							uint64_t size,
							uint64_t fat_size);
void				s_file_free_fat(t_s_file *arch_lst);

t_s_file			*add_arch_fat32(t_s_file *file,
							struct fat_arch *f32,
							t_s_file *arch_lst);
t_s_file			*add_arch_fat64(t_s_file *file,
							struct fat_arch_64 *f64,
							t_s_file *arch_lst);

struct load_command	*lc_parser(t_s_file *file, uint32_t cmd);
t_s_symbol			*sym_parser(t_s_file *file, struct load_command *sym_com);

t_s_symbol			*sort_alnum_rev(t_s_symbol *new, t_s_symbol *lst);
t_s_symbol			*sort_alnum(t_s_file *file,
							t_s_symbol *new,
							t_s_symbol *lst);
t_s_symbol			*lst_unsort(t_s_symbol *new, t_s_symbol *lst);

int					sect_parser(t_s_file *file, void *lc, uint32_t *s_index);
int					sect_parser_64(t_s_file *file, void *lc, uint32_t *s_index);

void				print_lst_sym(t_s_file *file, t_s_symbol *l, int nfiles);
void				nm_print_32(t_s_file *file, t_s_symbol *l, char c);
void				nm_print_64(t_s_file *file, t_s_symbol *l, char c);
void				print_lst_sym_u(t_s_file *file, t_s_symbol *l,
							const int nfiles);
void				print_file_name(t_s_file *file, const int nfiles);
int					print_section_text(t_s_file *file);
void				free_lst_sym(t_s_symbol *lst);
t_s_symbol			*add_lst_sym(t_s_file *file, t_s_symbol *lst,
							char *sym_name, void *lst_sym);

t_s_file			*fat_parse_arch(t_s_file *file, uint32_t narch);
t_s_file			*parse_archive(t_s_file *file,
							t_s_file *obj_lst, void *ptr);

uint16_t			swap_uint16(uint16_t nb);
uint32_t			swap_uint32(uint32_t nb);
uint64_t			swap_uint64(uint64_t nb);

#endif
