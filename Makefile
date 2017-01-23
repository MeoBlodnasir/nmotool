# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aduban <marvin@42.fr>                      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2014/12/02 12:05:34 by aduban            #+#    #+#              #
#    Updated: 2017/01/23 15:53:17 by aduban           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME_NM = ft_nm
NAME_OTOOL = ft_otool

SRC_NM = nm.c\
	  elems.c\
	  fat_archive.c\
	  sections.c\
	  utils.c\
	  elems_32.c\
	  sections_32.c\
	  utils_32.c\
	  swap_stuff.c\
	  swap_more_stuff.c\
	  handle_binary.c

SRC_OTOOL = otool.c\
			otool_sections.c\
			otool_utils.c\
			otool_sections_32.c\
			otool_fat_archive.c\
			otool_swap_stuff.c\
			otool_swap_more_stuff.c\
			otool_utils_32.c

.PHONY: libft.a

OBJ_NM = $(SRC_NM:.c=.o)
OBJ_OTOOL = $(SRC_OTOOL:.c=.o)

all : libft.a $(NAME_NM) $(NAME_OTOOL)

libft.a:
		@$(MAKE) -C ./libft

$(NAME_NM) : $(OBJ_NM)
		@gcc -Wall -Werror -Wextra -o $(NAME_NM) $(OBJ_NM) -L./libft/ -lftprintf -I ./libft
$(NAME_OTOOL) : $(OBJ_OTOOL)
		@gcc -Wall -Werror -Wextra -o $(NAME_OTOOL) $(OBJ_OTOOL) -L./libft/ -lftprintf -I ./libft

%.o: %.c
		@gcc -Wall -Werror -Wextra -I./libft/ -o $@ -c $^

clean :
		rm -f $(OBJ_NM) $(OBJ_OTOOL)

fclean : clean
		rm -f $(NAME_NM) $(NAME_OTOOL)

re : fclean all
