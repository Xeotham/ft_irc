#include "Server.hpp"

bool Server::_signal = false;

Server::Server(const int port, const std::string &password)
{
	std::srand(time(0));
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
        _port = rhs._port;
        _serverSocketFd = rhs._serverSocketFd;
        _clients = rhs._clients;
        _fds = rhs._fds;
        _password = rhs._password;
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
	struct sockaddr_in	clientAdress = {};
	struct pollfd		newPoll = {};
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

bool	Server::passCheck(int fd, std::string line)
{
	Client &user = Client::getClientByFd(_clients, fd);
	if (line == "LS 302")
		return true;
	if (line != _password && !_password.empty())
	{
		std::string message = "Error : wrong password.\r\n";
		send(fd, message.c_str(), message.size(), 0);
		std::cout << "Client <" << fd << "> disconnected." << std::endl;
		close(fd);
		clearClients(fd);
		return false;
	}
	user.setPassword();
	return true;
}

void    Server::quitCommand(int fd)
{
	std::cout << "Client " << fd << " disconnected." << std::endl;
	clearClients(fd);
	close(fd);
}


void	Server::userCommand(int fd, std::string line)
{
//	USER <username> <hostname> <servername> :<realname>
	Client &user = Client::getClientByFd(_clients, fd);
	line.resize(line.find_first_of(' '));
	user.setUser(line);
	std::cout << "Client <" << fd << "> set username to : " << user.getUser() << std::endl;
}

bool	Server::nickCommand(int fd, std::string line)
{
	for (size_t i = 0; i < _clients.size(); i++)
	{
		if (_clients[i].getNick() == line)
		{
			std::string message = "Error : nickname already used.\r\n";
			send(fd, message.c_str(), message.size(), 0);
			std::cout << "Client <" << _clients[i].getFd() << "> disconnected." << std::endl;
			close(fd);
			clearClients(fd);
			return false;
		}
	}
	Client &user = Client::getClientByFd(_clients, fd);
	user.setNick(line);
	std::cout << "Client <" << fd << "> set nickname to : " << user.getNick() << std::endl;
	return true;
}

void	Server::privMsgCommand(int fd, std::string line)
{
	std::string message;
	size_t pos = line.find(' ');
	std::string dest = line.substr(0, pos);
	std::cout << "dest : " << dest << std::endl;
	Client &user = Client::getClientByFd(this->_clients, fd);
	if (dest.find_first_of('#') != std::string::npos) {
		Channel &chan = Channel::getChannelByName(this->_channels, dest);
		for (std::vector<Client>::iterator it = chan.getUsers().begin(); it != chan.getUsers().end(); it++) {
			// message = ":" + user.getNick() + "!" + user.getUser() + "@localhost " + "PRIVMSG " + line + "\r\n";
			message = user.getSendMsg("PRIVMSG", line);
			if (it->getFd() != fd) {
				std::cout << "Client <" << fd << "> send message to <" << it->getNick() << "> : " << message <<
						std::endl;
				send(it->getFd(), message.c_str(), message.size(), 0);
			}
		}
	} else {
		message = user.getSendMsg("PRIVMSG", line);
		for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); it++) {
			if (it->getNick() == dest) {
				send(it->getFd(), message.c_str(), message.size(), 0);
				std::cout << "Client <" << fd << "> send message to <" << it->getNick() << "> : " << message << std::endl;
				return ;
			}
		}
		std::string err_message = "Error : user not found.\r\n";
		send(fd, err_message.c_str(), err_message.size(), 0);
	}
}

void Server::joinCommand(int fd, std::string line)
{
	Client &user = Client::getClientByFd(this->_clients, fd);
	std::cout << user.getNick() << " try to join" << std::endl;
	try {
		Channel &chan = Channel::getChannelByName(this->_channels, line);
		chan.addUser(user);
		std::cout << "Channel already exist" << std::endl;
	} catch (std::exception &exc) {
		try {
			Channel chan(line);
			chan.addUser(user);
			this->_channels.push_back(chan);
		} catch (std::exception &e) {
			std::string msg = e.what();
			msg += "\r\n";
			send(fd, msg.c_str(), msg.size(), 0);
			return;
		}
	}
	std::string msg = user.getSendMsg("JOIN", line);
	send(fd, msg.c_str(), msg.size(), 0);
}

bool	Server::checkData(int fd, std::string data)
{
	Client &user = Client::getClientByFd(this->_clients, fd);
	while (size_t pos = data.find_first_of('\n'))
	{
		if (data.size() < 2)
			return true;
		std::string line = data.substr(0, pos + 1);
		data.erase(0, line.size());
		size_t pos2 = line.find_first_of(' ');
		std::string command = line.substr(0, pos2);
		line.erase(0, pos2 + 1);
		line.resize(line.size() - 2);

		if (!user.getPassword())
		{
			if (!passCheck(fd, line))
				return false;
		}
		else if (command == "PRIVMSG")
			privMsgCommand(fd, line);
		else if (command == "QUIT")
		{
			quitCommand(fd);
			return false;
		}
		else if (command == "NICK")
		{
			if (!nickCommand(fd, line))
				return false;
		}
		else if (command == "USER")
			userCommand(fd, line);
		else if (command == "JOIN")
			joinCommand(fd, line);
		else if (command == "bot")
			Bot::botCommand(fd, line, _clients);
	}
	return true;
}

void	Server::receiveNewData(int fd)
{
	char	tmp[1024];

	std::memset(tmp, 0,sizeof(tmp));

	size_t data = recv(fd, tmp, sizeof(tmp) - 1, 0);
	/*if (data <= 0)
	{
		std::cout << "Client " << fd << " disconnected." << std::endl;
		clearClients(fd);
		close(fd);
		return ;
	}*/
	if (data > 0)
	{
		for (size_t i = 0; i < _clients.size(); i++)
		{
			if (_clients[i].getFd() == fd)
			{
				_clients[i].setBuffer(_clients[i].getBuffer() + tmp + '\0');
				if (_clients[i].getBuffer().find("\r\n") == std::string::npos)
					return ;
				std::cout << "Client " << fd << " > data : " << _clients[i].getBuffer() << std::endl;
				if (checkData(fd, _clients[i].getBuffer()))
					_clients[i].setBuffer("");
			}
		}
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
	for (size_t i =	0; i < _fds.size(); i++)
	{
		if (_fds[i].fd == fd)
		{
			_fds.erase(_fds.begin() + i);
			break;
		}
	}
	for (size_t i =	0; i < _clients.size(); i++)
	{
		if (_clients[i].getFd() == fd)
		{
			_clients.erase(_clients.begin() + i);
			break;
		}
	}
}

void	Server::serverSocket()
{
	struct sockaddr_in	serverAdress = {};
	struct pollfd		newPoll = {};

	serverAdress.sin_family = AF_INET;
	serverAdress.sin_port = htons(_port);
	serverAdress.sin_addr.s_addr = INADDR_ANY;

	_serverSocketFd = socket(AF_INET, SOCK_STREAM, 0);
	if (_serverSocketFd == -1)
		throw (std::runtime_error("Error : socket creation."));
	int opt = 1;
	if (setsockopt(_serverSocketFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
		throw (std::runtime_error("Error : cannot setup socket option (SO_REUSEADDR)."));
	if (fcntl(_serverSocketFd, F_SETFL, O_NONBLOCK) == -1)
		throw (std::runtime_error("Error : cannot setup option (O_NONBLOCK)."));
	if (bind(_serverSocketFd, (const struct sockaddr *) &serverAdress, sizeof(serverAdress)) == -1)
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
	while (!Server::_signal)
	{
		if ((poll(&_fds[0], _fds.size(), -1) == -1) && !Server::_signal)
			throw (std::runtime_error("Error : poll() failed."));
		for (size_t i = 0; i < _fds.size(); i++)
		{
			std::cout << "i : " << i << " fds size : " << _fds.size() << std::endl;
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
