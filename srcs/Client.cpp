#include "Client.hpp"

Client::Client()
{
	_password = false;
	_nick = "";
	_user = "";
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

void	Client::setBuffer(std::string buffer)
{
	this->_buffer = buffer;
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

std::string	Client::getBuffer() const
{
	return this->_buffer;
}