#ifndef MESSAGES_HPP
# define MESSAGES_HPP

# include "Params.hpp"
# include "Client.hpp"

# define MSG			"PRIVMSG"
# define JOIN			"JOIN"
# define PART			"PART"
# define NICK			"NICK"
# define USER			"USER"
# define KICK			"KICK"
# define WHO			"WHO"
# define QUIT			"QUIT"
# define BOT_CMD		"bot"
# define PING			"PING"
# define KICK			"KICK"
# define TOPIC			"TOPIC"
# define MODE			"MODE"
# define INVITE			"INVITE"
# define CAP			"CAP"
# define NAMES			"NAMES"
# define LIST			"LIST"
# define END_OF_ARRAY	""


# define WHOREPLY	"352"
# define ENDOFWHO	"315"
# define NAMREPLY	"353"
# define ENDOFNAMES	"366"
# define LISTSTART	"321"
# define LIST_RPL	"322"
# define LISTEND	"323"

class	Client;

class	Messages
{
	public:
		static void	sendMsg(int fd, const std::string &msg, const Client &sender, const std::string &type);
		static void	sendServMsg(int fd, const std::string &msg, const std::string &type);
		static void sendGlobalMsg(UserLst& users, const std::string &msg, const Client &sender, const std::string &type);
		static void sendGlobalMsg(UserPtrLst& users, const std::string &msg, const Client &sender, const std::string &type);

	private:
		Messages();
		Messages(const Messages &ref);
		~Messages();
	private:
		Messages	&operator=(const Messages &ref);
};

#endif