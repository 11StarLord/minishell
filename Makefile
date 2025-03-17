NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror -Iincludes
RM = rm -rf

LIBFT_DIR = ./libs/libft
LIBFT = $(LIBFT_DIR)/libft.a

SRCS =  src/main.c \
		src/init.c \
		src/env/duplicate_env.c \
		src/utils/readline.c \
		src/utils/release_memory.c \
		src/token/process_expansion.c \
		src/token/lexer.c \
		src/token/lex_process.c \
		src/token/extract_token.c \
		src/token/calculate_expansion.c \
		src/token/process_quote.c \
		src/env/get_env.c\
		src/token/utils.c\


OBJS = $(SRCS:.c=.o)

$(NAME): $(OBJS) $(LIBFT)
		@$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -lreadline -o $(NAME)
		clear

all: $(NAME)

$(LIBFT):
		@make -C $(LIBFT_DIR)
clean: 
		@$(RM) $(OBJS)
		@make clean -C $(LIBFT_DIR)
		clear

fclean: clean
		$(RM) $(NAME)
		@make fclean -C $(LIBFT_DIR)
		clear

re: fclean all

.PHONY: all clean fclean re
	