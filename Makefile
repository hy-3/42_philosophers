NAME = philo
CC = gcc
FLAG_WARN = -Wall -Wextra -Werror #-fsanitize=thread -g3 #-fsanitize=address
FLAG_PTHREAD = -lpthread
SRC = main.c \
      philo_checker.c \
      thread_philo.c \
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