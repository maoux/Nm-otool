/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   otool_print.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heynard <heynard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/22 18:45:35 by heynard           #+#    #+#             */
/*   Updated: 2018/12/25 18:24:57 by heynard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

static void			print_unsigned(size_t addr, size_t base, size_t len)
{
	static char		hex_str[] = "0123456789abcdef";
	char			value;

	if (len <= 0)
		return ;
	print_unsigned(addr / base, base, len - 1);
	value = hex_str[(addr % base)];
	write(1, &value, 1);
}

static int			print_section_text_32(t_s_file *file,
											uint32_t i,
											uint32_t j)
{
	void			*data_offset;
	uint8_t			data;

	data_offset = (void *)(file->map + file->offset_text_sect);
	while (i < file->size_text_sect.val_32)
	{
		j = -1;
		print_unsigned(file->addr_text_sect.val_32 + i, 16, 8);
		ft_putchar('\t');
		while (++j < 16)
		{
			data = *(uint8_t *)(data_offset + j);
			ft_printf("%02x ", file->cigam ? swap_uint32(data) : data);
			i++;
			if (i >= file->size_text_sect.val_32)
			{
				ft_putchar('\n');
				return (EXIT_SUCCESS);
			}
		}
		ft_putchar('\n');
		data_offset += 16;
	}
	return (EXIT_SUCCESS);
}

static int			print_section_text_64(t_s_file *file,
											uint32_t i,
											uint32_t j)
{
	void			*data_offset;
	uint8_t			data;

	data_offset = (void *)(file->map + file->offset_text_sect);
	while (i < file->size_text_sect.val_64)
	{
		j = -1;
		print_unsigned(file->addr_text_sect.val_64 + i, 16, 16);
		ft_putchar('\t');
		while (++j < 16)
		{
			data = *(uint8_t *)(data_offset + j);
			ft_printf("%02x ", file->cigam ? swap_uint32(data) : data);
			i++;
			if (i >= file->size_text_sect.val_64)
			{
				ft_putchar('\n');
				return (EXIT_SUCCESS);
			}
		}
		ft_putchar('\n');
		data_offset += 16;
	}
	return (EXIT_SUCCESS);
}

/*
** print ppc fcking norme
*/

static int			pppc(t_s_file *file,
											uint32_t i,
											uint32_t j,
											void *data_offset)
{
	uint32_t		data;

	while (i < file->size_text_sect.val_64)
	{
		j = -1;
		print_unsigned(file->addr_text_sect.val_64 + i, 16,
			file->format == MACHO_32BITS ? 8 : 16);
		ft_putchar('\t');
		while (++j < 16)
		{
			data = *(uint8_t *)(data_offset + j);
			ft_printf("%02x", data);
			if (j == 3 || j == 7 || j == 11 || j == 15)
				ft_putchar(' ');
			i++;
			if (i >= file->size_text_sect.val_64)
			{
				ft_putchar('\n');
				return (EXIT_SUCCESS);
			}
		}
		ft_putchar('\n');
		data_offset += 16;
	}
	return (EXIT_SUCCESS);
}

int					print_section_text(t_s_file *file)
{
	if (file->is_archive)
		ft_printf("%s(%s):\n", file->name, file->obj_name);
	else if (file->narch > 1)
	{
		if (file->ppc && file->format == MACHO_32BITS)
			ft_printf("%s (architecture ppc):\n", file->name);
		else if (file->ppc && file->format == MACHO_64BITS)
			ft_printf("%s (architecture ppc64):\n", file->name);
		else if (file->format == MACHO_32BITS)
			ft_printf("%s (architecture i386):\n", file->name);
		else if (file->format == MACHO_64BITS)
			ft_printf("%s (architecture x86_64):\n", file->name);
	}
	else
		ft_printf("%s:\n", file->name);
	ft_printf("Contents of (__TEXT,__text) section\n");
	if (file->ppc)
		return (pppc(file, 0, 0, (void *)(file->map + file->offset_text_sect)));
	if (file->format == MACHO_32BITS)
		return (print_section_text_32(file, 0, 0));
	return (print_section_text_64(file, 0, 0));
}
