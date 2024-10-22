#include <Channel.hpp>
#include <algorithm>
#include <Client.hpp>
#include <sstream>

Channel::Channel() {
	this->_name = "";
}

Channel::Channel(const std::string &name) {
	if (name.at(0) != '#' || name.size() > 50 || name.size() < 2
		|| name.find_first_of(" ,\007:") != std::string::npos)
		throw (std::invalid_argument("Invalid channel name."));
	this->_name = name;
}

Channel::Channel(const std::string &name, const std::string &pwd) {
	if (name.at(0) != '#' || name.size() > 50 || name.size() < 2
		|| name.find_first_of(" ,\007:") != std::string::npos)
		throw (std::invalid_argument("Invalid channel name."));
	this->_name = name;
	this->_password = pwd;
	std::cout << "Password: " << pwd << std::endl;
}

Channel::Channel(const Channel &other) {
	*this = other;
}

Channel &Channel::operator=(const Channel &other) {
	if (this == &other)
		return (*this);
	this->_name = other._name;
	this->_users = other._users;
	this->_password = other._password;
	return (*this);
}

Channel::~Channel() {
}

const std::string &Channel::getName() const {
	return (this->_name);
}

void Channel::setName(const std::string &name) {
	this->_name = name;
}

// Other member functions
void Channel::addUser(Client &user) {
	for (UserLst::iterator it = this->_users.begin(); it != this->_users.end(); it++) {
		if (it->getFd() == user.getFd())
			throw (std::invalid_argument("The user is already in the " + this->getName() + " channel."));
	}
	this->_users.push_back(user);
}

void Channel::removeUser(const Client &user) {
	for (UserLst::iterator it = this->_users.begin(); it != this->_users.end(); it++) {
		if (it->getFd() == user.getFd()) {
			this->_users.erase(it);
			return ;
		}
	}
	throw (std::invalid_argument("The user is not in the channel."));
}

std::vector<Client> &Channel::getUsers() {
	return (this->_users);
}

const std::string &Channel::getPassword() const {
	return (this->_password);
}

Channel &Channel::getChannelByName(std::vector<Channel> &lst, const std::string &name) {
	for (ChannelLst::iterator it = lst.begin(); it != lst.end(); it++) {
		if (it->getName() == name) {
			return (*it);
		}
	}
	throw (std::invalid_argument("Channel not found."));
}

bool	Channel::isUserInChannel(Channel &channel, const Client &user) {
	for (UserLst::const_iterator it = channel.getUsers().begin(); it != channel.getUsers().end(); it++) {
		if (it->getFd() == user.getFd())
			return (true);
	}
	return (false);
}