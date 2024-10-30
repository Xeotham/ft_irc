#ifndef MESSAGES_HPP
# define MESSAGES_HPP

# include "Params.hpp"
# include "Client.hpp"

# define MSG "PRIVMSG"
# define JOIN "JOIN"
# define PART "PART"
# define NICK "NICK"
# define USER "USER"
# define KICK "KICK"

class	Client;

class	Messages
{
	public:
		static void	sendMsg(int fd, const std::string &msg, const Client &sender, const std::string &type);
		static void sendGlobalMsg(UserLst& users, const std::string &msg, const Client &sender, const std::string &type);
	private:
		Messages();
		Messages(const Messages &ref);
		~Messages();
	private:
		Messages	&operator=(const Messages &ref);
};

#endif