#ifndef SERVER_HPP
# define SERVER_HPP

# include "Params.hpp"
# include "Client.hpp"
# include "Bot.hpp"
# include "Channel.hpp"
# include "Colors.hpp"

class Server
{
	private:
		int							_port;
		std::string					_password;
		int							_serverSocketFd;
		std::vector<struct pollfd>	_fds;
		UserLst				 		_clients;
		ChannelLst					_channels;
		static bool					_signal;

		void			serverSocket();

		void			acceptNewClient();
		void			receiveNewData(int fd);
		bool			checkData(int fd, const std::string &data);

		void			setNickCommand(int fd, std::string data);
		void			setUserCommand(int fd, std::string data);
		bool			passCheck(int fd, std::string data);

		static void		clearClients(int fd);

		void			addChannel(const std::string &name);

	public:
		Server(int port, const std::string &password);
		Server(const Server &ref);
		Server& operator=(const Server &rhs);
		~Server();
		
		void			serverInit();
		void			closeFds();
		static void		signalHandler(int signum);
};



#endif