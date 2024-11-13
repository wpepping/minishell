NAME := minishell
CC := cc
CFLAGS := -Wall -Wextra -Werror -g
LIBS = -Llibft -lreadline -lft
SRCDIR = src
OBJDIR = objs
SRCS = src/builtin2.c src/builtin3.c src/builtin.c src/builtin_utils.c src/cleanup2.c src/cleanup.c \
src/envp.c src/execution.c src/expand_envs.c src/heredoc.c src/manage_processes.c src/minishell.c \
src/parse.c src/parse_handlers.c src/parse_utils.c src/path.c src/preprocessing.c src/preprocessing_checks.c \
src/process_utils.c src/runcmd.c src/signal_handlers.c src/tokenization.c src/token_utils.c src/utils2.c \
src/utils3.c src/utils.c
OBJS := $(SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	$(MAKE) -C libft
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(NAME)

all: $(NAME)

clean:
	$(MAKE) -C libft fclean
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
