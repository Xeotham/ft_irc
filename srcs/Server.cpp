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

	if (line.size() < 5)
	{
		std::string message = "Error : wrong password.\r\n";
		send(fd, message.c_str(), message.size(), 0);
		clearClients(_fds, _clients, fd);
		return false;
	}

	std::string cmd = line.substr(0,4);
	if (cmd != "PASS")
	{
		std::string message = "Error : wrong password.\r\n";
		send(fd, message.c_str(), message.size(), 0);
		clearClients(_fds, _clients, fd);
		return false;
	}

	std::string passwd = line.substr(5);
	if (passwd != _password && !_password.empty())
	{
		std::string message = "Error : wrong password.\r\n";
		send(fd, message.c_str(), message.size(), 0);
		clearClients(_fds, _clients, fd);
		return false;
	}
	user.setPassword();
	return true;
}

bool	Server::checkData(int fd, const std::string &data)
{
	ACommand			*cmd = NULL;
	std::stringstream	storage(data);
	std::string			segment;
	Client		&user = Client::getClientByFd(this->_clients, fd);

	while (std::getline(storage, segment, '\n') && !segment.empty()) {
		if (!user.getUser().empty() && !user.getNick().empty() && !user.getIsSet())
			user.setIsSet();
		if (segment.find('\r') != std::string::npos)
			segment.erase(segment.find('\r'));
		if (segment[0] == 0)
			continue ;
		if (segment == "CAP LS 302")
			continue ;
		if (!user.getPassword())
		{
			if (!passCheck(fd, segment))
				return (false);
			continue ;
		}
		try
		{
			cmd = ACommand::cmdSelector(fd, this->_clients, this->_channels, segment);
			cmd->execute(fd);
			delete cmd;
		}
		catch (bool b) {
			if (!b)
				clearClients(_fds, _clients, fd);
			return (b);
		}
		catch (Error &e) {
			if (cmd)
				delete cmd;
			if (e.getType() == ERR_NICKNAMEINUSE)
			{
				e.sendError();
				clearClients(_fds, _clients, fd);
				return false;
			}
			e.sendError();
		}
	}
	return (true);
}

void	Server::receiveNewData(int fd)
{
	char	tmp[1024];

	std::memset(tmp, 0,sizeof(tmp));

	size_t data = recv(fd, tmp, sizeof(tmp) - 1, 0);

	if (data <= 0)
	{
		clearClients(_fds, _clients, fd);
		return ;
	}
	for (size_t i = 0; i < _clients.size(); i++)
	{
		if (_clients[i].getFd() == fd)
		{
			_clients[i].setBuffer(_clients[i].getBuffer() + tmp);
			if (_clients[i].getBuffer().find("\r\n") == std::string::npos)
				return ;
			if (checkData(fd, _clients[i].getBuffer()))
				_clients[i].setBuffer("");
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

void	Server::clearClients(std::vector<struct pollfd> &_fds, UserLst &_clients, int fd)
{
	JoinCmd	cmd(Client::getClientByFd(_clients, fd), _clients, _channels, "0");

	cmd.execute(fd);
	std::cout << "Client <" << fd << "> disconnected." << std::endl << std::endl;
	close(fd);
	for (size_t i =    0; i < _fds.size(); i++)
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
			if (_fds[i].revents & POLLIN)
			{
				if (_fds[i].fd == _serverSocketFd)
					acceptNewClient();
				else
					receiveNewData(_fds[i].fd);
			}
		}
			std::cout << std::endl << RED "SERVER DATA:" CLR<< std::endl;
			std::cout << "Current clients:" << std::endl;
			for (UserLst::iterator it = _clients.begin(); it != _clients.end(); ++it) {
				std::cout << "Client FD: " << it->getFd() << ", IP: " << it->getIpAdd() << std::endl;
			}

			std::cout << "Current channels:" << std::endl;
			for (ChannelLst::iterator itt = _channels.begin(); itt != _channels.end(); ++itt) 
			{
				std::cout << GRN "Channel Name: " << itt->getName()<< CLR << std::endl;
				UserLst &users = itt->getUsers();
				for (UserLst::iterator aa = users.begin(); aa != users.end(); ++aa) {
					std::cout << "	Client FD: " << aa->getFd() << ", IP: " << aa->getIpAdd() << std::endl;
				}
				UserLst &operators = itt->getOperators();
				for (UserLst::iterator ittt = operators.begin(); ittt != operators.end(); ++ittt) {
					std::cout << "	Operator FD: " << ittt->getFd() << ", IP: " << ittt->getIpAdd() << std::endl;
				}
			}
			std::cout << std::endl<< std::endl;
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
