############################		Variables		############################
OK_COLOR=\x1b[32;01m
DANGER_COLOR=\x1b[1;31m
YELLOW_COLOR=\x1b[0;33m
NO_COLOR=\x1b[0m

LEMIN = lem-in
VISUAL = visualiser

INCS_DIR = ./includes
LIBS_DIR = ./libs
OBJS_DIR = ./objs
SRCS_DIR = ./srcs

LIBS = libft libftprintf libdraw

SRC_MAIN_DIR =		/core
SRC_MAIN =		main.c				debug.c				show_map_config.c \
				get_paths_diff.c	paths_utils.c		find_comb.c \
				find_unique_paths.c	find_best_comb_paths.c \
				let_ants_to_paths.c	check_intersections.c

SRC_VISUAL_DIR =	/visual
SRC_VISUAL =	main.c				draw_farm.c			draw_ant.c \
				animation_ant.c		ft_close.c			move_farm.c

COMMON_SRC =	read_args.c			throw_error.c		find_node_by_name.c \
				utils.c				read_farm_map.c		create_room_and_edge.c \
				create_sort_room_arr.c

OBJS_MAIN += $(addprefix $(OBJS_DIR)/$(SRC_MAIN_DIR)/,$(SRC_MAIN:%.c=%.o))
OBJS_MAIN += $(addprefix $(OBJS_DIR)/,$(COMMON_SRC:%.c=%.o))
OBJS_VISUAL += $(addprefix $(OBJS_DIR)/$(SRC_VISUAL_DIR)/,$(SRC_VISUAL:%.c=%.o))
OBJS_VISUAL += $(addprefix $(OBJS_DIR)/,$(COMMON_SRC:%.c=%.o))

FLAGS += -Wall -Wextra -Werror
FLAGS += -g
FLAGS += -O2
FLAGS += -I$(INCS_DIR)
FLAGS += $(foreach lib,$(LIBS),-I$(LIBS_DIR)/$(lib)/includes)

BIN_FLAGS += $(FLAGS)
BIN_FLAGS += $(foreach lib,$(LIBS),-L$(LIBS_DIR)/$(lib) -l$(lib:lib%=%))

VISUAL_FLAGS += -framework OpenGL -framework AppKit

############################		  Rules 		############################

all: $(LIBS) $(LEMIN) $(VISUAL)

$(LIBS):
	@$(MAKE) -C $(LIBS_DIR)/$@

$(OBJS_DIR):
	@mkdir -p $@
	@mkdir -p $@/$(SRC_MAIN_DIR)
	@mkdir -p $@/$(SRC_VISUAL_DIR)

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c | $(OBJS_DIR)
	@gcc $(FLAGS) -c $< -o $@

$(LEMIN): $(OBJS_MAIN)
	@gcc -o $@ $^ $(BIN_FLAGS)
	@echo "$(OK_COLOR)Build $@ complete$(NO_COLOR)"

$(VISUAL): $(OBJS_VISUAL)
	@gcc -o $@ $^ $(BIN_FLAGS) $(VISUAL_FLAGS)
	@echo "$(OK_COLOR)Build $@ complete$(NO_COLOR)"

clean:
	@rm -rf $(OBJS_DIR)
	@echo "$(DANGER_COLOR)Object files cleared$(NO_COLOR)"

fclean: clean
	@rm -f $(LEMIN)
	@rm -f $(VISUAL)
	@echo "$(DANGER_COLOR)Build cleared$(NO_COLOR)"

re: fclean all

.PHONY: all re clean fclean

#	by rschuppe