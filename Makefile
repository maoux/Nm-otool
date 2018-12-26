# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: heynard <heynard@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/05/20 21:04:52 by heynard           #+#    #+#              #
#    Updated: 2018/12/24 18:42:58 by heynard          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME_NM = ft_nm
NAME_OT = ft_otool

NAME_LIB = lib_ft

CC = gcc
IFLAG = -I ./includes/ -I ./libft/includes/
CFLAG = -Wall -Wextra -Werror
LFLAG = -L ./libft -lft

SRC_PATH = ./srcs/

SRC_COMMON = ft_error.c\
			options_parser.c\
			s_file_handler.c\
			check_memory.c\
			lc_parser.c\
			sym_parser.c\
			lst_sym.c\
			sect_parser.c\
			swap.c\
			fat_parse_arch.c\
			s_file_handler_fat.c\
			fat_add_lst_arch.c\
			sort_lst_sym.c\
			archive_parser.c\
			nm_print.c\
			nm_print_2.c
SRCS_COMMON = $(addprefix $(SRC_PATH), $(SRC_COMMON))
OBJ_COMMON = $(SRCS_COMMON:.c=.o)

SRC_NM = ft_nm.c\
		nm_file.c
SRCS_NM = $(addprefix $(SRC_PATH), $(SRC_NM))
OBJ_NM = $(SRCS_NM:.c=.o)

SRC_OT = ft_otool.c\
		otool_print.c\
		otool_file.c
SRCS_OT = $(addprefix $(SRC_PATH), $(SRC_OT))
OBJ_OT = $(SRCS_OT:.c=.o)

all: $(NAME_LIB) $(NAME_NM) $(NAME_OT)

$(NAME_LIB):
	make -C ./libft

$(NAME_NM): $(OBJ_NM) $(OBJ_COMMON)
	$(CC) $(CFLAG) $(LFLAG) -o $@ $(OBJ_NM) $(OBJ_COMMON)

$(NAME_OT): $(OBJ_OT) $(OBJ_COMMON)
	$(CC) $(CFLAG) $(LFLAG) -o $@ $(OBJ_OT) $(OBJ_COMMON)

%.o: %.c
	@$(CC) -o $@ -c $< $(CFLAG) $(IFLAG)
	@printf "\033[1;32m[ 0K ]\033[0m\tCompiling (C) $@\n"

.PHONY: all clean fclean re

clean:
	@make -C ./libft clean
	rm -rf $(OBJ_NM) $(OBJ_OT) $(OBJ_COMMON)

fclean: clean
	@make -C ./libft fclean
	rm -rf $(NAME_NM) $(NAME_OT)

re: fclean all
