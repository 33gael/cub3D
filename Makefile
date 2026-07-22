NAME        = cub3D

CC          = cc
CFLAGS      = -Wall -Wextra -Werror -g

BLUE        = \033[0;36m
RESET       = \033[0m

SRCS_DIR    = srcs
INC_DIR     = includes
LIBFT_DIR   = includes/libft
MINILIBX_DIR= minilibx-linux

SRCS        = $(SRCS_DIR)/exec/main.c \
              $(SRCS_DIR)/exec/init.c \
              $(SRCS_DIR)/exec/draw.c \
              $(SRCS_DIR)/exec/raycast.c \
              $(SRCS_DIR)/exec/math_utils.c \
              $(SRCS_DIR)/exec/player_move.c \
              $(SRCS_DIR)/exec/player_hooks.c \
              $(SRCS_DIR)/parsing/parser.c \
              $(SRCS_DIR)/parsing/player_init.c \
              $(SRCS_DIR)/parsing/textures.c \
              $(SRCS_DIR)/parsing/colors.c \
              $(SRCS_DIR)/parsing/useful_functions.c

OBJS        = $(SRCS:.c=.o)

LIBFT       = $(LIBFT_DIR)/libft.a
INC         = -I $(INC_DIR) -I $(LIBFT_DIR) -I $(MINILIBX_DIR)
LIBS        = -L $(MINILIBX_DIR) -lmlx -L $(LIBFT_DIR) -lft -lXext -lX11 -lm -lz

all: $(LIBFT) $(MINILIBX_DIR)/libmlx.a $(NAME)

$(LIBFT):
	@make -C $(LIBFT_DIR)

$(MINILIBX_DIR):
	@printf "$(BLUE)MiniLibX cloning$(RESET)\n"
	@git clone https://github.com/42Paris/minilibx-linux.git $(MINILIBX_DIR)

$(MINILIBX_DIR)/libmlx.a: | $(MINILIBX_DIR)
	@make -C $(MINILIBX_DIR)

$(NAME): $(OBJS)
	@clear
	@$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(NAME)
	@printf "$(BLUE)"
	@echo '              __   _____ ____  '
	@echo '  _______  __/ /_ |__  // __ \ '
	@echo ' / ___/ / / / __ \ /_ </ / / / '
	@echo '/ /__/ /_/ / /_/ /__/ / /_/ /  '
	@echo '\___/\__,_/_.___/____/_____/   '
	@printf "$(RESET)\n"

%.o: %.c
	@$(CC) $(CFLAGS) $(INC) -c $< -o $@

clean:
	@make -C $(LIBFT_DIR) clean
	@if [ -d "$(MINILIBX_DIR)" ]; then make -C $(MINILIBX_DIR) clean; fi
	@rm -f $(OBJS)

fclean: clean
	@make -C $(LIBFT_DIR) fclean
	@rm -rf $(NAME) $(MINILIBX_DIR)

re: fclean all

.PHONY: all clean fclean re
