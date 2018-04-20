##
## EPITECH PROJECT, 2017
## Bistro-matic
## File description:
## Makefile for bistro-matic
##

SRC	=	test.c		\

OBJ	=	$(SRC:.c=.o)

NAME	=	evo

CFLAGS	=	-lc_graph_prog -g2

all:	$(NAME)

$(NAME): $(OBJ)
	gcc -o $(NAME) $(OBJ) -lc_graph_prog

clean:
	rm -f $(OBJ)

fclean:	clean
	rm -f $(NAME)

re:	fclean all

.PHONY: all re clean fclean
