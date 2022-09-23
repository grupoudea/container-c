SRCS	=	main.c

OBJS	=	$(SRCS:.c=.o)

CC	=	gcc

CFLAGS	=	-W -Wextra -Wall -Werror 

NAME	=	main

all:	$(OBJS)
	$(CC) -o $(NAME) $(OBJS)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: clean fclean re
