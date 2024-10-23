#ifndef SERVER_HPP
# define SERVER_HPP

# include "Params.hpp"
# include "Client.hpp"
# include "Bot.hpp"
# include "Channel.hpp"

class Server
{
	private:
		int							_port;
		int							_serverSocketFd;
		UserLst				 		_clients;
		std::vector<struct pollfd>	_fds;
		ChannelLst					_channels;
		static bool					_signal;
		std::string					_password;

		void		serverSocket();

		void		acceptNewClient();
		void		receiveNewData(int fd);
		void		checkData(int fd, const std::string &data);

		void		setNickCommand(int fd, std::string data);
		void		setUserCommand(int fd, std::string data);
		bool		passCheck(int fd, std::string data);

		void		clearClients(int fd);

		void		addChannel(const std::string &name);

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