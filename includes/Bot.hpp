#ifndef BOT_HPP
# define BOT_HPP

# include <Params.hpp>
# include "Client.hpp"
# include "Server.hpp"
# include <netdb.h>

# define BOT Client("Bot", "Bot")

class Bot
{
private:
	Bot();
	Bot(const Bot& other);
	Bot& operator=(const Bot& other);

	~Bot();
	static void		sendCommand(int fd, Client &user);
	static void		sendJoke(int fd, Client &user);
	static void		sendPong(int fd, Client &user);

public:
	static void    botCommand(int fd, std::string data, std::vector<Client> &vec);

};

typedef void (*t_func)(int, Client &);

#endif