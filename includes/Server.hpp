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
# include <sstream>
# include <netdb.h>
# include <cstdlib>
# include <ctime>

# include "Client.hpp"
# include "Bot.hpp"

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

		void		acceptNewClient();
		void		receiveNewData(int fd);
		void		checkData(int fd, std::string data);

		void		privMsgCommand(int fd, std::string data);
		void		setNickCommand(int fd, std::string data);
		void		setUserCommand(int fd, std::string data);
		bool		passCheck(int fd, std::string data);

		void		clearClients(int fd);
	public:
		Server(const int port, const std::string &password);
		Server(const Server &ref);
		Server& operator=(const Server &rhs);
		~Server();
		
		void	serverInit();
		void	closeFds();
		static void		signalHandler(int signum);
};



#endif