#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: banthony <banthony@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/08/17 11:57:23 by banthony          #+#    #+#              #
#    Updated: 2019/08/18 19:03:56 by abara            ###   ########.fr        #
#                                                                              #
#******************************************************************************#

NAME		= Matt_daemon	# Binary name
DIRINC		= ./include
DIRSRC		= ./src
DIROBJ		= ./

SRC			+=	main.cpp
SRC			+=	Tintin_reporter.cpp
SRC			+=	Daemon.cpp
SRC			+=	Server.cpp
SRC			+=	Utils.cpp

OBJ			=	$(SRC:.cpp=.o)
OBJS		=	$(OBJ:%=$(DIROBJ)%)

CC			=	/usr/bin/clang++
RM			=	/bin/rm -f
ECHO		=	/bin/echo

CFLAGS		=	-Wall -Wextra -Werror -I $(DIRINC)

all		:		$(NAME)

$(NAME)	:		$(OBJ)
				$(CC) $(CFLAGS) -o $(NAME) $(OBJS)
				@$(ECHO) '> Compiled'

clean	:
				@(cd $(DIROBJ) && $(RM) $(OBJ))
				@$(ECHO) '> Directory cleaned'

fclean	:		clean
				-@$(RM) $(NAME) #'-' en cas d'erreur ne pas stop le makefile ; '@' silence la premiere ligne d'output (instruction rm ...)
				@$(ECHO) '> Remove executable'

re		:		fclean all

%.o		:		$(DIRSRC)/%.cpp
				$(CC) $(CFLAGS) -o $(DIROBJ)$@ -c $<
