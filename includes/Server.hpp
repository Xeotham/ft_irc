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

# include "Client.hpp"

class Server
{
	private:
		int							_port;
		int							_serverSocketFd;
		std::string					_password;
		std::vector<Client> 		_clients;
		std::vector<struct pollfd>	_fds;
		static bool					_signal;
	public:
		Server();
		Server(const int port, const std::string &password);
		~Server();
		
		void		serverSocket();
		void		serverInit();

		static void	signalHandler(int signum);

		void		closeFds();
		void		clearClients(int fd);
};



#endif