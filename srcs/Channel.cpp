#include <Channel.hpp>
#include <algorithm>
#include <Client.hpp>

Channel::Channel() {
	this->_name = "";
}

Channel::Channel(const std::string& name) {
	if (name.at(0) != '#' && name.size() > 50 && name.size() < 2
		&& name.find_first_of(" ,\007:") != std::string::npos)
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
	for (std::vector<Client *>::iterator it = this->_users.begin(); it != this->_users.end(); it++)
		if (*it == &user)
			throw (std::invalid_argument("Client already exists."));
	this->_users.push_back(&user);
}

void Channel::removeUser(const Client& user) {

	std::vector<Client *>::iterator it;
	for (it = this->_users.begin(); it != this->_users.end(); it++)
		if (*it == &user)
			break;
	if (it != this->_users.end())
		this->_users.erase(it);
}

const std::vector<Client*>	&Channel::getUsers() const {
	return (this->_users);
}

// std::vector<Channel>::const_iterator	&Channel::getChannelByName(std::vector<Channel> lst, const std::string &name)
// {
// 	for (std::vector<Channel>::iterator it = lst.begin(); it != lst.end(); it++)
// 		if (it->getName() == name)
// 			return (it);
// 	return (lst.end());
// }