#include "Client.hpp"

Client::Client()
{
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

void Client::setChannel(const Channel &new_channel) {
	if (&this->getChannel() != &new_channel)
		this->_current_channel = new_channel;
	else
		throw (std::invalid_argument("User is already in the channel."));
}

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
	this->_password = 1;
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

int	Client::getPassword() const
{
	return this->_password;
}

