# Makefile for ft_malcolm

NAME = ft_malcolm

CC = gcc
CFLAGS = -Wall -Wextra -Werror
INCLUDES = -Iincludes

SRC_DIR = src
SRCS = $(SRC_DIR)/main.c \
       $(SRC_DIR)/parse.c \
       $(SRC_DIR)/socket_utils.c \
       $(SRC_DIR)/arp_handler.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
