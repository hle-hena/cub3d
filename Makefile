MAKEFLAGS += --no-print-directory

NAME = cub3d
CC = cc 
FLAGS = -Wall -Wextra -Werror -I./include -Imlx -mavx2
RM = rm -f

FRAMEDIR = ./mlx
FRAMEWORK = -Lmlx -lmlx -Imlx -lXext -lX11 -lm -lz -lXfixes

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

SRCS = $(addprefix srcs/, \
	$(addprefix clean/, clean_data.c clean_map.c)\
	$(addprefix utils/, add_link.c)\
	$(addprefix parser/, parser.c\
		$(addprefix tile_dict/, build_polygon.c \
			hash_map.c is_dict_full.c retrieve_lonely.c \
			retrieve_player.c retrieve_texture_color.c retrieve_texture_val.c \
			retrieve_texture.c retrieve_tile_dict.c retrieve_tile_info.c \
			retrieve_value.c retrieve_wall_line.c)\
		$(addprefix map/, is_map_valid.c retrieve_map.c)\
		$(addprefix lmap/, load.c retrieve_light.c)\
		$(addprefix raytrace/, find_flight.c handle_light.c raytrace_utils.c\
			raytrace.c reflect_light.c wall_intersect_light.c)\
	)\
	$(addprefix mlx_utils/, color.c loop.c put_pixel.c)\
	$(addprefix game/, \
		$(addprefix draw/, \
			$(addprefix mini_map/, draw_circle.c draw_line.c draw_mini_map.c \
				draw_player.c draw_square.c)\
			$(addprefix raycast/, draw_map.c lookup.c raycast_utils.c \
				raycast.c reflexion.c wall_intersect.c)\
		)\
		$(addprefix hooks/, key.c loop.c mouse.c)\
		$(addprefix status/, fps.c)\
	)\
	main.c debug.c\
)

OBJ = $(SRCS:.c=.o)

.c.o:
	@printf "\r\033[K\033[34mCompiling: $<\033[0m"
	@$(CC) $(FLAGS) -c $< -o $(<:.c=.o)
	@printf "\r\033[K"

all: FLAGS += -O3
all: $(NAME)

no: clear
no: $(NAME)

debug: clear
debug: FLAGS += -g
debug: $(NAME)

fdebug: clear
fdebug: FLAGS += -fsanitize=address -fno-omit-frame-pointer -g
fdebug: $(NAME)

valgrind: all
	valgrind --leak-check=full --show-leak-kinds=all ./$(NAME) $(ARG)

$(LIBFT):
	@echo
	@make -C $(LIBFT_DIR)

$(NAME): $(LIBFT) $(OBJ)
	@echo "\n\033[2K\r\033[94mLinking $(NAME) 🗹\033[0m\n"
	@echo "\033[2K\r\033[94mLinking mlx 🗹\033[0m\n"
	@make -C mlx > /dev/null 2>&1
	@$(CC) $(FLAGS) $(OBJ) $(FRAMEWORK) -o $(NAME) -L $(LIBFT_DIR) -lft
	@len=$$(echo -n "$(NAME)" | wc -c); \
	border=$$(printf '=%.0s' $$(seq 1 $$((len + 36)))); \
	echo "\n\033[94m|$$border|\033[0m"; \
	echo "\033[94m|    🎉 $(NAME) Compilation Complete! 🎉    |\033[0m"; \
	echo "\033[94m|$$border|\033[0m\n"

clear:
	@echo -n "\033[34m"
	@$(RM) $(OBJ)
	@echo "Object files removed\033[0m"

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

.PHONY: all clean fclean clear re debug fdebug
