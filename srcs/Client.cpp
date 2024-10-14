#include "Client.hpp"
// #include "Channel.hpp

Client::Client()
{
	this->_fd = -1;
	this->_ipAdd = "";
	this->_nick = "";
	this->_user = "";
	this->_password = false;
}

Client::Client(const Client &ref)
{
	*this = ref;
}

Client	&Client::operator=(const Client &ref)
{
	if (this == &ref)
		return *this;
	this->_fd = ref._fd;
	this->_ipAdd = ref._ipAdd;
	// this->_current_channel = ref._current_channel;
	this->_nick = ref._nick;
	this->_user = ref._user;
	this->_password = ref._password;
	return *this;
}

Client::~Client()
{
}

void	Client::setFd(int fd)
{
	this->_fd = fd;
}

void	Client::setIpAdd(std::string ip)
{
	this->_ipAdd = ip;
}

// void Client::setChannel(Channel &new_channel) {
// 	if (this->getChannel() != &new_channel)
// 		this->_current_channel = &new_channel;
// 	else
// 		throw (std::invalid_argument("User is already in the channel."));
// }

// Channel *Client::getChannel() const {
// 	return (this->_current_channel);
// }

int		Client::getFd() const
{
	return this->_fd;
}

void	Client::setNick(std::string nick)
{
	this->_nick = nick;
}

void	Client::setUser(std::string user)
{
	this->_user = user;
}

void	Client::setPassword()
{
	this->_password = true;
}

std::string	Client::getIpAdd() const
{
	return this->_ipAdd;
}

std::string	Client::getNick() const
{
	return this->_nick;
}

std::string	Client::getUser() const
{
	return this->_user;
}

bool	Client::getPassword() const
{
	return this->_password;
}

Client	&Client::getClientByFd(std::vector<Client> &lst, int fd)
{
	for (std::vector<Client>::iterator it = lst.begin(); it != lst.end(); it++) {
		if (it->getFd() == fd) {
			return (*it);
		}
	}
	throw (std::invalid_argument("Client not found."));
}
