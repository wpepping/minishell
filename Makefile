CC := gcc

CFLAGS := -Wall -Wextra -Werror -g
LIBS = -lreadline


# SRCS := minishell.c\
# 		parse.c\
# 		utils.c\
# 		buildtin.c

SRCS = $(wildcard *.c) $(wildcard libft/*.c)


OBJS := $(SRCS:.c=.o)

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

NAME := minishell

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(NAME) 
 
all: $(NAME)


 
clean:
	@rm -f $(OBJS)
	
fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re

