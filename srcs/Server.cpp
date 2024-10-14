#include "Server.hpp"

bool Server::_signal = false;

Server::Server(int port, const std::string &password)
{
	_port = port;
	_serverSocketFd = -1;
	_password = password;
}

Server::Server(const Server &ref)
{
	this->_port = 0;
	this->_serverSocketFd = 0;
	*this = ref;
}

Server& Server::operator=(const Server &rhs)
{
	(void)rhs;
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
	socklen_t			len = sizeof(clientAdress);

	int	clientSocketFd = accept(_serverSocketFd,(sockaddr *)&clientAdress, &len);
	if (clientSocketFd == -1)
		throw (std::runtime_error("Error : cannot accept client."));
	if (fcntl(clientSocketFd, F_SETFL, O_NONBLOCK) == -1)
		throw (std::runtime_error("Error : cannot setup option (O_NONBLOCK)."));

	newPoll.fd = clientSocketFd;
	newPoll.events = POLLIN;
	newPoll.revents = 0;

	cl.setFd(clientSocketFd);
	cl.setIpAdd(inet_ntoa(clientAdress.sin_addr));
	_clients.push_back(cl);
	_fds.push_back(newPoll);

	std::cout << "New client <" << clientSocketFd << "> connected." << std::endl;
}

bool	Server::passCheck(int fd, std::string data)
{
	size_t pos = data.find("PASS");
	data.erase(0, pos + 5);
	size_t pos2 = data.find("\r\n");
	if (data.substr(0, pos2) != _password)
	{
		std::string message = "Error : wrong password.\r\n";
		send(fd, message.c_str(), message.size(), 0);
		close(fd);
		clearClients(fd);
		return false;
	}
	for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		if (it->getFd() == fd)
			it->setPassword();
	}
	return true;
}

void	Server::setUserCommand(int fd, std::string data)
{
	size_t pos = data.find("USER");
	data.erase(0, pos + 5);
	size_t pos2 = data.find(" ");
	for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		if (it->getFd() == fd)
		{
			it->setUser(data.substr(0, pos2));
			std::cout << "Client <" << fd << "> set user to : " << it->getUser() << std::endl;
		}
	}
	return ;
}

void	Server::setNickCommand(int fd, std::string data)
{
	size_t pos = data.find("NICK");
	data.erase(0, pos + 5);
	size_t pos2 = data.find("\r\n");
	for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		if (it->getFd() == fd)
		{
			it->setNick(data.substr(0, pos2));
			std::cout << "Client <" << fd << "> set nick to : " << it->getNick() << std::endl;
		}
	}
	return ;
}

	for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		std::cout << "Client <" << it->getFd() << ">." << std::endl;
	}
}

void	Server::receiveNewData(int fd)
{
	char	buffer[1024];
	
	std::memset(buffer, 0,sizeof(buffer));

	int data = recv(fd, buffer, sizeof(buffer) - 1, 0);
	if (data <= 0)
	{
		std::cout << "Client " << fd << " disconnected." << std::endl;
		clearClients(fd);
		close(fd);
	}
	else
	{
		buffer[data] = '\0';
		std::cout << "Client " << fd << " > data : " << buffer << std::endl;
	}
	Server::checkData(fd, buffer);
}

void	Server::closeFds()
{
	for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); it++)
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
	for (std::vector<struct pollfd>::iterator it = _fds.begin(); it != _fds.end(); it++)
	{
		if (it->fd == fd)
		{
			_fds.erase(it);
			break;
		}
	}
	for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		if (it->getFd() == fd)
		{
			_clients.erase(it);
			break;
		}
	}
}

void	Server::serverSocket()
{
	struct sockaddr_in	serverAdress;
	struct pollfd		newPoll;

	serverAdress.sin_family = AF_INET;
	serverAdress.sin_port = htons(_port);
	serverAdress.sin_addr.s_addr = INADDR_ANY;

	_serverSocketFd = socket(AF_INET, SOCK_STREAM, 0);
	if (_serverSocketFd == -1)
		throw (std::runtime_error("Error : socket creation."));
	
	int	opt = 1;
	if (setsockopt(_serverSocketFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
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
	_fds.push_back(newPoll);
}


void	Server::serverInit()
{
	serverSocket();
	while (Server::_signal == false)
	{
		std::cout << "size " << _fds.size() << std::endl;
		if ((poll(&_fds[0], _fds.size(), -1) == -1) && Server::_signal == false)
			throw (std::runtime_error("Error : poll() failed."));
		std::cout << "Polling..." << std::endl;
		for (size_t i = 0; i < _fds.size(); i++)
		{
			if (_fds[i].revents & POLLIN)
			{
				if (_fds[i].fd == _serverSocketFd)
					acceptNewClient();
				else
					receiveNewData(_fds[i].fd);
			}
		}
	}
	closeFds();
}

void	Server::addChannel(const std::string &name)
{
	try {
		for (std::vector<Channel>::iterator it = _channels.begin(); it != _channels.end(); it++)
		{
			if (it->getName() == name)
				throw (std::invalid_argument("Channel already exists."));
		}
		Channel ch(name);
		_channels.push_back(ch);
	} catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
}

// void	Server::serverInit()
// {
// 	serverSocket();
// 	while (Server::_signal == false)
// 	{
// 		for (std::vector<struct pollfd>::iterator it = _fds.begin(); it != _fds.end(); it++)
// 		{
// 			if ((poll(&_fds[0], _fds.size(), -1) == -1) && Server::_signal == false)
// 				throw (std::runtime_error("Error : poll() failed."));
// 			if (it->revents & POLLIN)
// 			{
// 				if (it->fd == _serverSocketFd)
// 					acceptNewClient();
// 				else
// 					receiveNewData(it->fd);
// 			}
// 		}
// 	}
// 	closeFds();
// }