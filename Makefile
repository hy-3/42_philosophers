NAME = philo
CC = gcc
FLAG_WARN = -Wall -Wextra -Werror #-fsanitize=address -g3
FLAG_PTHREAD = -lpthread
SRC = main.c \
      thread_philo.c \
      thread_deadchecker.c \
      util.c \
      util2.c
OBJ = $(SRC:%.c=%.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) $(FLAG_WARN) $(FLAG_PTHREAD) -o $(NAME)

%.o: %.c
	$(CC) $(FLAG_WARN) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	make clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re