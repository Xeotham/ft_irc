#include <Channel.hpp>
#include <algorithm>

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
void Channel::addUser(const Client& user) {
	if (std::find(this->_users.begin(), this->_users.end(), user) == this->_users.end())
		throw (std::invalid_argument("Client already exists."));
	this->_users.push_back(user);
}

void Channel::removeUser(const Client& user) {

	std::vector<Client>::iterator it = std::find(this->_users.begin(), this->_users.end(), user);
	if (it != this->_users.end())
		this->_users.erase(it);
}

const std::vector<Client>& Channel::getUsers() const {
	return (this->_users);
}