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

OBJ_DIR = _objs
OBJS = $(SRCS:%.c=$(OBJ_DIR)/%.o)

$(OBJ_DIR)/%.o: %.c
	mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
		@$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -lreadline -o $(NAME)
		clear
		
$(LIBFT):
		@make -C $(LIBFT_DIR)
clean: 
		@$(RM) $(OBJ_DIR)
		@make clean -C $(LIBFT_DIR)
		clear

fclean: clean
		$(RM) $(NAME)
		@make fclean -C $(LIBFT_DIR)
		clear

re: fclean all

.PHONY: all clean fclean re
	