NAME = philo


CC = cc
CFLAGS = -Wall -Wextra -Werror
LDFLAGS = -lpthread
ASAN_FLAGS = -g -fsanitize=address

SRC = main.c philosopher.c utils.c monitor.c action.c action_2.c monitor_utils.c main_init.c main_valid.c main_create.c free.c utils_2.c utils_3.c
OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(LDFLAGS)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

# New rule for AddressSanitizer
fsanitize: CFLAGS += $(ASAN_FLAGS)
fsanitize: clean $(NAME)
	./$(NAME) $(filter-out $@,$(MAKECMDGOALS))

# New rule for Valgrind
valgrind: clean $(NAME)
	valgrind --leak-check=full --show-leak-kinds=all ./$(NAME) $(filter-out $@,$(MAKECMDGOALS))

# New rule for GDB
debug: CFLAGS += -g
debug: clean $(NAME)
	gdb --args ./$(NAME) $(filter-out $@,$(MAKECMDGOALS))

.PHONY: all clean fclean re fsanitize valgrind debug
