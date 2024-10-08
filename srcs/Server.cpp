#include "Server.hpp"

bool Server::_signal = false;

Server::Server(const int port, const std::string &password)
{
	_port = port;
	_serverSocketFd = -1;
	_password = password;
}

Server::Server(const Server &ref)
{
	*this = ref;
}

Server& Server::operator=(const Server &rhs)
{
	if (this != &rhs)
	{
		_port = 
	}
	return *this;
}

Server::~Server()
{
}

void	Server::signalHandler(int signum)
{
	(void)signum;
	std::cout << "Signal received." << std::endl;
	Server::_signal = true;
}

void		Server::acceptNewClient()
{
	Client cl;
	struct sockaddr_in	clientAdress;
	struct pollfd		newPoll;

}

void	Server::closeFds()
{
	for (std::vector<Client>::iterator it = this->_clients.begin(); it != this->_clients.end(); it++)
	{
		std::cout << "Client <" << it->getFd() << "> disconnected." << std::endl;
		close(it->getFd());
	}
	if (_serverSocketFd != -1)
	{
		std::cout << "Server <" << _serverSocketFd << "> disconnected." << std::endl;
		close(_serverSocketFd);
	}
}

void	Server::clearClients(int fd)
{
	for (std::vector<struct pollfd>::iterator it = this->_fds.begin(); it != this->_fds.end(); it++)
	{
		if (it->fd == fd)
		{
			this->_fds.erase(it);
			break;
		}
	}
	for (std::vector<Client>::iterator it = this->_clients.begin(); it != this->_clients.end(); it++)
	{
		if (it->getFd() == fd)
		{
			this->_clients.erase(it);
			break;
		}
	}
}

void	Server::serverSocket()
{
	struct sockaddr_in	serverAdress;
	struct pollfd		newPoll;

	serverAdress.sin_family = AF_INET;
	serverAdress.sin_port = htons(this->_port);
	serverAdress.sin_addr.s_addr = INADDR_ANY;

	this->_serverSocketFd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_serverSocketFd == -1)
		throw (std::runtime_error("Error : socket creation."));
	
	int	opt = 1;
	if (setsockopt(_serverSocketFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt) == -1))
		throw (std::runtime_error("Error : cannot setup socket option (SO_REUSEADDR)."));
	if (fcntl(_serverSocketFd, F_SETFL, O_NONBLOCK) == -1)
		throw (std::runtime_error("Error : cannot setup option (O_NONBLOCK)."));
	if (bind(_serverSocketFd, (const struct sockaddr *)&serverAdress, sizeof(serverAdress)) == -1)
		throw (std::runtime_error("Error : cannot bind socket."));
	if (listen(_serverSocketFd, SOMAXCONN) == -1)
		throw (std::runtime_error("Error : cannot listen socket."));

	newPoll.fd = _serverSocketFd;
	newPoll.events = POLLIN;
	newPoll.revents = 0;
	this->_fds.push_back(newPoll);
}

void	Server::serverInit()
{
	serverSocket();
}