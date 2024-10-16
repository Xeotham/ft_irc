#include <Channel.hpp>
#include <algorithm>
#include <Client.hpp>

Channel::Channel() {
	this->_name = "";
}

Channel::Channel(const std::string& name) {
	if (name.at(0) != '#' || name.size() > 50 || name.size() < 2
		|| name.find_first_of(" ,\007:") != std::string::npos)
		throw (std::invalid_argument("Invalid channel name."));
	this->_name = name;
}

Channel::Channel(const Channel& other) {
	*this = other;
}

Channel& Channel::operator=(const Channel& other) {
	if (this == &other)
		return (*this);
	this->_name = other._name;
	this->_users = other._users;
	return (*this);
}

Channel::~Channel() {}

const std::string& Channel::getName() const {
	return (this->_name);
}

void Channel::setName(const std::string& name) {
	this->_name = name;
}

// Other member functions
void Channel::addUser(Client& user) {
	for (std::vector<Client>::iterator it = this->_users.begin(); it != this->_users.end(); it++) {
		if (it->getFd() == user.getFd())
			throw (std::invalid_argument("The user is already in the channel."));
	}
	this->_users.push_back(user);
	std::cout << user.getNick() << " pushed back in " << this->getName() << std::endl;
}

void Channel::removeUser(const Client& user) {

	std::vector<Client>::iterator it;
	for (it = this->_users.begin(); it != this->_users.end(); it++) {
		if (&(*it) == &user)
			this->_users.erase(it);
	}
}

std::vector<Client>	&Channel::getUsers() {
	return (this->_users);
}

Channel	&Channel::getChannelByName(std::vector<Channel> &lst, const std::string &name)
{
	for (std::vector<Channel>::iterator it = lst.begin(); it != lst.end(); it++) {
		if (it->getName() == name.c_str())
			return (*it);
	}
	throw (std::invalid_argument("Channel not found."));
}