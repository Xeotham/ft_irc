#ifndef BOTCMD_HPP
# define BOTCMD_HPP

# include "Params.hpp"
# include "Client.hpp"
# include "Server.hpp"
# include <netdb.h>
# include "ACommand.hpp"

# define BOT Client("Bot", "Bot")

class BotCmd : public ACommand
{
private:
	BotCmd();
	BotCmd(const BotCmd& other);
	BotCmd& operator=(const BotCmd& other);

	void		sendJoke(int fd, Client &user);
	void		sendPong(int fd, Client &user);

public:
	BotCmd(Client &user, UserLst &user_lst, ChannelLst &chan_lst, const std::string &data);
	~BotCmd();
	void    execute(int fd);
};

typedef void (*t_func)(int, Client &);

#endif