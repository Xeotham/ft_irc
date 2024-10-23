#include <Server.hpp>
#include <JoinCmd.hpp>
#include <PrivMsgCmd.hpp>
#include <PartCmd.hpp>

#include "NickCmd.hpp"

bool Server::_signal = false;

Server::Server(int port, const std::string &password)
{
	std::srand(time(0));
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
	Client &user = Client::getClientByFd(_clients, fd);
	if (user.getPassword() || ((data.find("CAP LS 302") != std::string::npos) && (data.find("PASS") == std::string::npos) && (data.find("NICK") == std::string::npos)))
		return true;
	if ((data.find("CAP LS 302") != std::string::npos) && (data.find("PASS") == std::string::npos) && (data.find("NICK") != std::string::npos))
	{
		std::string message = "Error : wrong password.\r\n";
		send(fd, message.c_str(), message.size(), 0);
		std::cout << "Client <" << fd << "> disconnected." << std::endl;
		close(fd);
		clearClients(fd);
		return false;
	}
	size_t pos = data.find("PASS");
	data.erase(0, pos + 5);
	size_t pos2 = data.find("\r\n");
	if (data.substr(0, pos2) != _password)
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

bool	Server::checkData(int fd, const std::string &data)
{
	ACommand							*cmd = __nullptr;
	std::stringstream					storage(data);
	std::string							segment;
	if (!passCheck(fd, data))
		return (false);
	while (std::getline(storage, segment, '\n') && !segment.empty()) {

		if (segment.find('\r') != std::string::npos)
			segment.resize(segment.size() - 1);
		try {
			cmd = 	ACommand::cmdSelector(this->_clients, this->_channels, segment);
			cmd->execute(fd);
			delete cmd;
		}
		catch (bool b) {
			return (b);
		}
		catch (std::exception &e) {
			std::cerr << e.what() << std::endl;
		}
	}
	return (true);
	// if (data.find("PRIVMSG") != std::string::npos) {
	// 	PrivMsgCmd	cmd;
	// 	data.erase(0, 8);
	// 	data.resize(data.size() - 2);
	// 	cmd.execute(fd, data, this->_channels, this->_clients);
	// }
	// if (data.find("QUIT") != std::string::npos)
	// 	return ;
	// if (data.find("NICK") != std::string::npos) {
	// 	// setNickCommand(fd, data.substr(5, data.size() - 2));
	// 	NickCmd    cmd;
	// 	data.erase(0, 5);
	// 	data.resize(data.size() - 2);
	// 	cmd.execute(fd, data, this->_channels, this->_clients);
	// }
	// if (data.find("USER") != std::string::npos)
	// 	setUserCommand(fd, data);
	// if (data.find("JOIN") != std::string::npos) {
	// 	JoinCmd	cmd;
	// 	data.erase(0, 5);
	// 	data.resize(data.size() - 2);
	// 	cmd.execute(fd, data, this->_channels, this->_clients);
	// }
	// if (data.find("bot") != std::string::npos)
	// 	Bot::botCommand(fd, data, _clients);
	// if (data.find("PART") != std::string::npos) {
	// 	PartCmd	cmd;
	// 	data.erase(0, 5);
	// 	data.resize(data.size() - 2);
	// 	cmd.execute(fd, data, this->_channels, this->_clients);
	// }
}

void	Server::receiveNewData(int fd)
{
	char	tmp[1024];

	std::memset(tmp, 0,sizeof(tmp));

	int data = recv(fd, tmp, sizeof(tmp) - 1, 0);
	/*if (data <= 0)
	{
		std::cout << "Client " << fd << " disconnected." << std::endl;
		clearClients(fd);
		close(fd);
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
	struct sockaddr_in	serverAdress;
	struct pollfd		newPoll;

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
