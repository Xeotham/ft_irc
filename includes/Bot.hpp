#ifndef BOT_HPP
# define BOT_HPP

# include "Params.hpp"
# include "Client.hpp"
# include "Server.hpp"
# include <netdb.h>
# include "ACommand.hpp"

# define BOT Client("Bot", "Bot")

class Bot : public ACommand
{
private:
	Bot();
	Bot(const Bot& other);
	Bot& operator=(const Bot& other);

	void		sendCommand(int fd, Client &user);
	void		sendJoke(int fd, Client &user);
	void		sendPong(int fd, Client &user);

public:
	Bot(UserLst &user_lst, ChannelLst &chan_lst, const std::string &data);
	~Bot();
	void    execute(int fd);
};

typedef void (*t_func)(int, Client &);

#endif