#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <sys/socket.h>
# include <sys/types.h>
# include <netinet/in.h>
# include <poll.h>
# include <fcntl.h>
# include <arpa/inet.h>
# include <unistd.h>
# include <vector>
# include <csignal>
# include <limits>
# include <cstring>
# include <cstdlib>

# include "Client.hpp"

class Server
{
	private:
		int							_port;
		int							_serverSocketFd;
		std::vector<Client> 		_clients;
		std::vector<struct pollfd>	_fds;
		static bool					_signal;
		std::string					_password;

		void		serverSocket();
		void		signalHandler(int signum);

		void		acceptNewClient();
		void		receiveNewData(int fd);

		void		joinCommand();

		void		clearClients(int fd);
	public:
		Server(const int port, const std::string &password);
		Server(const Server &ref);
		Server& operator=(const Server &rhs);
		~Server();
		
		void	serverInit();
		void	closeFds();
};



#endif