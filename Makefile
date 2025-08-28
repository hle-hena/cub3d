MAKEFLAGS += --no-print-directory

NAME = cub3d
CC = cc
FLAGS = -Wall -Wextra -Werror -I./include -Imlx -O3
RM = rm -f

FRAMEDIR = ./mlx
FRAMEWORK = -Lmlx -lmlx -Imlx -lXext -lX11 -lm -lz -lXfixes

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

SRCS = $(addprefix srcs/, \
	$(addprefix clean/, clean_data.c clean_map.c)\
	$(addprefix utils/, add_link.c)\
	$(addprefix parser/, parser.c\
		$(addprefix tile_dict/, hash_map.c is_dict_full.c retrieve_lonely.c \
			retrieve_player.c retrieve_texture_color.c retrieve_texture.c \
			retrieve_tile_dict.c retrieve_tile_info.c retrieve_value.c)\
		$(addprefix map/, is_map_valid.c retrieve_map.c)\
	)\
	$(addprefix mlx_utils/, color.c loop.c put_pixel.c)\
	$(addprefix game/, \
		$(addprefix draw/, \
			$(addprefix mini_map/, draw_circle.c draw_line.c draw_map.c \
				draw_player.c draw_square.c)\
			$(addprefix raycast/, draw_line.c draw_map.c lookup.c raycast.c)\
		)\
		$(addprefix status/, fps.c)\
		$(addprefix hooks/, key.c loop.c mouse.c)\
	)\
	main.c\
)

OBJ = $(SRCS:.c=.o)

.c.o:
	@printf "\r\033[K\033[34mCompiling: $<\033[0m"
	@$(CC) $(FLAGS) -c $< -o $(<:.c=.o)
	@printf "\r\033[K"

all: $(NAME)

valgrind: all
	valgrind --leak-check=full --show-leak-kinds=all ./$(NAME) $(ARG)

$(LIBFT):
	@echo
	@make -C $(LIBFT_DIR)

$(NAME): $(LIBFT) $(OBJ)
	@echo "\n\033[2K\r\033[94mLinking $(NAME) 🗹\033[0m\n"
	@echo "\033[2K\r\033[94mLinking mlx 🗹\033[0m\n"
	@make -C mlx > /dev/null 2>&1
	@$(CC) $(OBJ) $(FRAMEWORK) -o $(NAME) -L $(LIBFT_DIR) -lft
	@len=$$(echo -n "$(NAME)" | wc -c); \
	border=$$(printf '=%.0s' $$(seq 1 $$((len + 36)))); \
	echo "\n\033[94m|$$border|\033[0m"; \
	echo "\033[94m|    🎉 $(NAME) Compilation Complete! 🎉    |\033[0m"; \
	echo "\033[94m|$$border|\033[0m\n"

clean:
	@echo -n "\033[34m"
	@make -C $(LIBFT_DIR) clean
	@make -C mlx/ clean > /dev/null
	@$(RM) $(OBJ)
	@echo "Object files removed\033[0m"

fclean: clean
	@echo -n "\033[34m"
	@make -C $(LIBFT_DIR) fclean > /dev/null
	@make -C mlx/ clean > /dev/null
	@$(RM) $(NAME)
	@echo "Binary files and Libft removed\033[0m"

re: fclean all

.PHONY: all clean fclean re
