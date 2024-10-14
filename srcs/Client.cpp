#include "Client.hpp"
// #include "Channel.hpp

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

void Client::setChannel(Channel &new_channel) {
	if (this->getChannel() != &new_channel)
		this->_current_channel = &new_channel;
	else
		throw (std::invalid_argument("User is already in the channel."));
}

Channel *Client::getChannel() const {
	return (this->_current_channel);
}

int		Client::getFd() const
{
	return this->_fd;
}

