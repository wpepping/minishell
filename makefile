CC := cc

CFLAGS := -Wall -Wextra -Werror


SRCS := minishell.c\
		parse.c\
		utils.c\
		buildtin.c


LIBFT = ./libftprintf.a

OBJS := $(SRCS:.c=.o)

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

NAME := minishell

$(NAME): $(OBJS) $(LIBFT) 
	@$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME) 
 
all: $(NAME) $(LIBFT)


$(LIBFT):
	@make -C ./ft_printf
 
clean:
	@rm -f $(OBJS)
	@make clean -C ./libft
	
fclean: clean
	@make fclean -C ./libft
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re bonus