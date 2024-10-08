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

int		Client::getFd() const
{
	return this->_fd;
}