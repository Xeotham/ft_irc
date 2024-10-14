#ifndef BOT_HPP
#define BOT_HPP

# include "Client.hpp"
# include "Server.hpp"
# include <netdb.h>

class Bot : public Client 
{
public:
	Bot();
	Bot(const Bot& other);
	Bot& operator=(const Bot& other);

	~Bot();

private:

	void	initBot(const std::string& server, int port, int &sockfd, std::string password);
	bool	sendToServer(int sockfd, const std::string& msg);
	bool	connectToServer(const std::string& server, int port, int &sockfd);
};

#endif // BOT_HPP