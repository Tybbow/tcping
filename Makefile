NAME = tcping
CC = gcc
CFLAGS = -Wall -Werror -Wextra -pthread
HEADERS = -I ./include/

SRC_PATH = ./srcs

SRCS =  tcping.c\
        options.c\
		send.c\
		sniffer.c\
		socket.c\
		thread.c\
        handle.c

OBJS = $(patsubst %.c,$(SRC_PATH)/%.o,$(SRCS))

$(NAME): $(OBJS)
	@($(CC) $(CFLAGS) $(HEADERS) -o $@ $^)
	@echo "--------------------------------------------------"
	@echo "          Compilation of $@ has finish            "
	@echo "               $@ : v 1.0 - By Tybbow            "
	@echo "              For Section Metrologie                "
	@echo "--------------------------------------------------"

all : $(NAME)

%.o: %.c
	@($(CC) -c $(CFLAGS) $(HEADERS) -o $@ $^)
	@echo " OK  $@"

clean:
	@(rm -rf $(OBJS))
	@echo "--------------------------------------------------"
	@echo " OK          $(NAME): Objects deleted"
	@echo "--------------------------------------------------"

fclean: clean
	@(rm -rf $(NAME))
	@echo "--------------------------------------------------"
	@echo " OK           $(NAME): All deleted"
	@echo "--------------------------------------------------"

re: fclean all

.PHONY: fclean clean all
