#include "Client.hpp"
#include "Channel.hpp"

Client::Client()
{
	this->_fd = -1;
	this->_ipAdd = "";
	this->_nick = "";
	this->_user = "";
	this->_is_set = false;
	this->_password = false;
}

Client::Client(const std::string &nick, const std::string &user)
{
	this->_fd = -1;
	this->_ipAdd = "";
	this->_nick = nick;
	this->_user = user;
	this->_is_set = false;
	this->_password = false;
}

Client::Client(const Client &ref)
{
	this->_fd = ref._fd;
	this->_password = ref._password;
	this->_is_set = false;
	*this = ref;
}

Client	&Client::operator=(const Client &ref)
{
	if (this == &ref)
		return *this;
	this->_fd = ref._fd;
	this->_ipAdd = ref._ipAdd;
	this->_nick = ref._nick;
	this->_user = ref._user;
	this->_password = ref._password;
	this->_channels = ref._channels;
	this->_is_set = ref._is_set;
	return (*this);
}

Client::~Client()
{
}

void	Client::setFd(int fd)
{
	this->_fd = fd;
}

void	Client::setIpAdd(const std::string &ip)
{
	this->_ipAdd = ip;
}

int		Client::getFd() const {
	return (this->_fd);
}
void	Client::setNick(const std::string &nick)
{
	this->_nick = nick;
}

void	Client::setUser(const std::string &user)
{
	this->_user = user;
}

void	Client::setRealname(const std::string &realname)
{
	this->_realname = realname;
}

void	Client::setPassword()
{
	this->_password = true;
}

void	Client::setBuffer(const std::string &buffer)
{
	this->_buffer = buffer;
}

void	Client::setIsSet()
{
	this->_is_set = true;
}

void	Client::addChannel(const Channel channel)
{
	this->_channels.push_back(channel);
}

void	Client::removeChannel(const Channel &channel)
{
	for (ChannelLst::iterator it = this->_channels.begin(); it != this->_channels.end(); it++) {
		if (it->getName() == channel.getName()) {
			this->_channels.erase(it);
			break;
		}
	}
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

std::string	Client::getRealname() const
{
	return this->_realname;
}

bool	Client::getPassword() const
{
	return this->_password;
}

std::string	Client::getBuffer() const
{
	return this->_buffer;
}

ChannelLst	&Client::getChannels()
{
	return (this->_channels);
}

bool	Client::getIsSet() const
{
	return this->_is_set;
}

Client	&Client::getClientByFd(UserLst &lst, int fd)
{
	for (std::vector<Client>::iterator it = lst.begin(); it != lst.end(); it++) {
		if (it->getFd() == fd) {
			return (*it);
		}
	}
	throw (std::invalid_argument("Client not found."));
}

Client	&Client::getClientByNick(UserLst &lst, const std::string &nick)
{
	for (std::vector<Client>::iterator it = lst.begin(); it != lst.end(); it++) {
		if (it->getNick() == nick) {
			return (*it);
		}
	}
	throw (std::invalid_argument("Client not found."));
}

Client	*Client::getClientByNickPt(UserLst &lst, const std::string &nick)
{
	for (std::vector<Client>::iterator it = lst.begin(); it != lst.end(); it++) {
		if (it->getNick() == nick) {
			return (&(*it));
		}
	}
	return (NULL);
}


Client	&Client::getClientByUser(UserLst &lst, const std::string &user)
{
	for (std::vector<Client>::iterator it = lst.begin(); it != lst.end(); it++) {
		if (it->getUser() == user) {
			return (*it);
		}
	}
	throw (std::invalid_argument("Client not found."));
}

bool	Client::isClientInList(UserLst &lst, const std::string &nick)
{
	for (UserLst::iterator it = lst.begin(); it != lst.end(); it++) {
		if (it->getNick() == nick)
			return (true);
	}
	return (false);
}
