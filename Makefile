# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: iiliuk <iiliuk@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2017/02/23 13:27:53 by iiliuk            #+#    #+#              #
#    Updated: 2017/02/23 17:36:26 by iiliuk           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ft_ls

CFLAGS = -Wall -Wextra -Werror

FILES = ft_ls.c sort_files.c help_funcs.c open_read_dirs.c \
display_args.c display_stats.c

FT_PRINTF = ft_printf.c helper.c get_flags.c process_char.c process_ptr.c \
process_digit_id.c process_digit_oux.c process_percent.c process_width.c process_precision.c \
process_wide_char.c

LIBFT = ft_memset.c ft_bzero.c ft_memcpy.c ft_memccpy.c ft_memmove.c \
ft_memchr.c ft_memcmp.c ft_strlen.c ft_strdup.c ft_strcpy.c ft_strncpy.c \
ft_strcat.c ft_strncat.c ft_strlcat.c ft_strchr.c ft_strrchr.c ft_strstr.c \
ft_strnstr.c ft_strcmp.c ft_strncmp.c ft_atoi.c ft_isalpha.c ft_isdigit.c \
ft_isalnum.c ft_isascii.c ft_isprint.c ft_toupper.c ft_tolower.c \
ft_memalloc.c ft_memdel.c ft_strnew.c ft_strdel.c ft_strclr.c ft_striter.c \
ft_striteri.c ft_strmap.c ft_strmapi.c ft_strequ.c ft_strnequ.c ft_strsub.c \
ft_strjoin.c ft_strtrim.c ft_strsplit.c ft_itoa.c ft_putchar.c ft_putstr.c \
ft_putendl.c ft_putnbr.c ft_putchar_fd.c ft_putstr_fd.c ft_putendl_fd.c \
ft_putnbr_fd.c ft_lstnew.c ft_lstdelone.c ft_lstdel.c ft_lstadd.c ft_lstiter.c \
ft_lstmap.c ft_list_size.c ft_list_add_back.c ft_list_last.c \
ft_list_push_params.c ft_list_nth.c ft_list_reverse.c ft_realloc.c ft_abs.c \
ft_itoa_base.c ft_strndup.c ft_itoa_unbase.c ft_lstdelcont.c ft_lst_free_cont.c \
ft_print_list.c

FILE_SRCS = $(addprefix srcs/,$(FILES))

FT_PRINTF_SRCS = $(addprefix srcs/ft_printf/,$(FT_PRINTF))

LIBFT_SRCS = $(addprefix srcs/libft/,$(LIBFT))

SRCS = $(FILE_SRCS) $(LIBFT_SRCS) $(FT_PRINTF_SRCS)

OBJECTS = $(FILES:.c=.o) $(LIBFT:.c=.o) $(FT_PRINTF:.c=.o)

HDR = -I./includes

.PHONY: all clean fclean re

RED			=	\033[0;31m
GREEN		=	\033[0;32m
NC			=	\033[0m

all: $(NAME)

$(NAME):
	@echo "$(GREEN)compiling executable...$(NC)"
	@gcc $(CFlAGS) $(HDR) -c $(SRCS)
	@gcc $(CFlAGS) -o $(NAME) $(OBJECTS)
	@echo "$(GREEN)./ft_ls is ready to test$(NC)"

clean:
	@echo "$(RED)deleting all objects...$(NC)"
	@/bin/rm -f $(OBJECTS)
	@make -C srcs/libft clean

fclean: clean
	@echo "$(RED)deleting executable and libraries...$(NC)"
	@/bin/rm -f $(NAME)
	@make -C srcs/libft fclean

re: fclean all
