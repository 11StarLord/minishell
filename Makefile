NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror -Iincludes
RM = rm -rf

LIBFT_DIR = ./libs/libft
LIBFT = $(LIBFT_DIR)/libft.a

SRCS =  src/main.c \
		src/init.c \
		src/utils/handle_signal.c \
		src/utils/readline.c \
		src/utils/release_memory.c \
		src/token/lexical/process_expansion.c \
		src/token/lexical/lexer.c \
		src/token/lexical/lex_process.c \
		src/token/lexical/extract_token.c \
		src/token/lexical/calculate_expansion.c \
		src/token/lexical/process_quote.c \
		src/token/semantic/reorganize_token.c \
		src/redir/handle_redirection.c \
		src/redir/pipe.c \
		src/token/token_analysis.c\
		src/token/utils.c\
		src/env/get_env.c\
		src/env/duplicate_env.c \
		src/env/env_to_matrix.c \
		src/token/semantic/handle_heredoc.c	\
		src/token/semantic/dup_token.c	\
		src/redir/verify_redir_syntax.c \
		src/exec/handle_execution.c 	\
		src/exec/handle_path_cmd.c 	\
		src/exec/exec_cmd_extern.c 	\
		src/exec/exec_cmd_builtin.c 	\
		src/exec/bult_in/echo.c 	\
		src/exec/bult_in/pwd.c 	\
		src/exec/bult_in/exit.c 	\
		src/exec/bult_in/util.c 	\
		src/exec/bult_in/unset.c 	\
		src/utils/reset_and_close_fd.c

OBJ_DIR = _objs
OBJS = $(SRCS:%.c=$(OBJ_DIR)/%.o)



all: $(NAME)

$(OBJ_DIR)/%.o: %.c
	mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

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
	