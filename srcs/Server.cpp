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

void	Server::privmsgCommand(int fd, std::string data)
{
	std::string message;
	data.erase(0, 8);
	size_t pos = data.find(' ');
	std::string dest = data.substr(0, pos);
	std::cout << "dest : " << dest << std::endl;
	if (dest.find_first_of('#') != std::string::npos)
	{
		Channel &chan = Channel::getChannelByName(this->_channels, dest);
		for (std::vector<Client>::iterator it = chan.getUsers().begin(); it != chan.getUsers().end() ; it++ ) {
			std::cout << it->getNick() << std::endl;
			send(it->getFd(), message.c_str(), message.size(), 0);
		}

		// std::string channel = dest;
		// std::string message = "Error : channel not supported.\r\n";
	}
	else
	{
		Client &value = Client::getClientByFd(this->_clients, fd);
		message = value.getNick() + " " + data.substr(pos + 1);
		for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); it++)
		{
			if (it->getNick() == dest)
			{
				send(it->getFd(), message.c_str(), message.size(), 0);
				std::cout << "Client <" << fd << "> send message to <" << it->getNick() << "> : " << message << std::endl;
				return ;
			}
		}
		std::string message = "Error : user not found.\r\n";
		send(fd, message.c_str(), message.size(), 0);
	}
}

void Server::joinCommand(int fd, std::string data) {
	data.erase(0, 5);
	data.erase(data.end() - 2, data.end());
	Client	&user = Client::getClientByFd(this->_clients, fd);
	std::cout << user.getNick() << " try to join" << std::endl;
	try {
		Channel &chan = Channel::getChannelByName(this->_channels, data);
		chan.addUser(user);
		std::cout << "Channel already exist" << std::endl;
	}
	catch (std::exception &exc) {
		try {
			Channel	chan(data);
			chan.addUser(user);
			this->_channels.push_back(chan);
		}
		catch (std::exception &e) {
			std::string	msg = e.what();
			msg += "\r\n";
			send(fd, msg.c_str(), msg.size(), 0);
			return ;
		}
	}
	std::string msg = ":" + user.getNick() + "!" + user.getUser() + "@localhost JOIN :" + data + "\r\n";
	send(fd, msg.c_str(), msg.size(), 0);
}

void	Server::checkData(int fd, std::string data)
{
	if (data.find("PASS") != std::string::npos)
		if (passCheck(fd, data) == false)
			return ;
	for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		if (it->getFd() == fd && it->getPassword() == false)
		{
			std::string message = "Error : wrong password.\r\n";
			send(fd, message.c_str(), message.size(), 0);
			std::cout << "Client <" << it->getFd() << "> disconnected." << std::endl;
			close(fd);
			clearClients(fd);
			return ;
		}
	}
	if (data.find("PRIVMSG") != std::string::npos)
		privmsgCommand(fd, data);
	else if (data.find("QUIT") != std::string::npos)
		return ;
	else if (data.find("NICK") != std::string::npos)
		setNickCommand(fd, data);
	else if (data.find("USER") != std::string::npos)
		setUserCommand(fd, data);
	else if (data.find("JOIN") != std::string::npos)
		joinCommand(fd, data);
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
		Server::checkData(fd, buffer);
	}
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
		if ((poll(&_fds[0], _fds.size(), -1) == -1) && Server::_signal == false)
			throw (std::runtime_error("Error : poll() failed."));
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
