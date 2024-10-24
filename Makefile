NAME = ircserv

SRCS = 	main.cpp \
		Client.cpp \
		Server.cpp \
		Channel.cpp \
		Messages.cpp \
		cmd/JoinCmd.cpp \
		cmd/PartCmd.cpp \
		cmd/ACommand.cpp \
		cmd/NickCmd.cpp \
		cmd/KickCmd.cpp \
		cmd/UserCmd.cpp
# cmd/PrivMsgCmd.cpp \
#Bot.cpp

SRCS_DIR = srcs/
OBJS_DIR = obj/
INCLUDE_DIR = includes/

OBJS = $(addprefix $(OBJS_DIR), $(SRCS:.cpp=.o))

CC = c++
LDFLAGS = $(LIBS)
FLAGS = -Wall -Wextra -Werror -g -std=c++98
LIBS = -I$(INCLUDE_DIR)

RM = rm -rf

all : $(NAME)

$(NAME) : $(OBJS)
	@$(CC) $(FLAGS) $(OBJS) $(LDFLAGS) -o $(NAME)

$(OBJS_DIR)%.o : $(SRCS_DIR)%.cpp
	@mkdir -p $(dir $@)
	@$(CC) $(FLAGS) $(LDFLAGS) -c $< -o $@

clean :
	@$(RM) $(OBJS_DIR)

fclean : clean
	@$(RM) $(NAME)

re : fclean all

.PHONY : all clean fclean re