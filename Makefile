NAME    = minishell
CC      = cc
CFLAGS  = -Wextra -Wall -Werror -g
LIBFT   = libft/
HEADER  = includes/
HEADERS = -I $(HEADER) -I $(LIBFT)

OBJ_DIR = obj

MAIN    = main.c utils.c process.c signal.c
PHASER  = phaser.c heredoc.c syntax.c syntax_con.c phaser_env.c phaser_re.c phaser_utils.c
HANDLE  = error_func.c all_error.c err_utils.c
BUILTINS = builtins.c echo.c env.c export.c exit.c cd.c unset.c built_utils.c

SRCS = $(addprefix srcs/main/, $(MAIN)) \
       $(addprefix srcs/phaser/, $(PHASER)) \
       $(addprefix srcs/handle/, $(HANDLE)) \
       $(addprefix srcs/builtins/, $(BUILTINS))

OBJS = $(SRCS:srcs/%.c=$(OBJ_DIR)/%.o)
OBJ_DIRS = $(sort $(dir $(OBJS)))

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)/libft.a $(HEADER)/minishell.h Makefile
	@echo "\e[0;36mCompiling minishell..."
	@$(CC) $(CFLAGS) $(HEADERS) -o $(NAME) $(OBJS) -L$(LIBFT) -lft -lreadline
	@echo "\e[0;32mDone!\e[0m"

$(LIBFT)/libft.a:
	@make -C $(LIBFT)

$(OBJ_DIR)/%.o: srcs/%.c | $(OBJ_DIRS)
	@$(CC) $(CFLAGS) -o $@ -c $< $(HEADERS) && printf "\e[0;34mCompiling: $(notdir $<)\n"

$(OBJ_DIRS):
	@mkdir -p $@

clean:
	@echo "\e[0;34mCleaning..."
	@make clean -C $(LIBFT)
	@rm -rf $(OBJ_DIR)
	@echo "\e[0;32mDone!\e[0m"

fclean: clean
	@echo "\e[0;34mCleaning all..."
	@make fclean -C $(LIBFT)
	@rm -f $(NAME)
	@echo "\e[0;32mDone!\e[0m"

re: fclean all

.PHONY: all clean fclean re libft
