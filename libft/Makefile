SRCS = ft_atol.c ft_atoi.c ft_calloc.c ft_isalnum.c ft_isalpha.c ft_isdigit.c ft_itoa.c ft_lstadd_back.c ft_lstadd_front.c ft_lstclear.c ft_lstdelone.c ft_lstfind.c ft_lstlast.c ft_lstnew.c ft_lstsize.c ft_memcpy.c ft_memset.c ft_putchar_fd.c ft_putendl_fd.c ft_putnbr_fd.c ft_putstr_fd.c ft_split.c ft_strchr.c ft_strdup.c ft_strjoin.c ft_strlen.c ft_strncmp.c ft_strrchr.c ft_substr.c

OBJS = $(patsubst %.c,%.o, $(SRCS))

NAME = libft.a

CFLAGS = -Wall -Werror -Wextra

all: $(NAME)

$(NAME): $(OBJS)
	ar rcs $(NAME) $(OBJS)

objs:
	cc -c $(CFLAGS) $(SRCS)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all
