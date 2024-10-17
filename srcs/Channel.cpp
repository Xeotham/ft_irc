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
	UserLst::iterator it = this->_users.begin();
	while (it != this->_users.end() && it->getFd() != user.getFd())
		it++;
	if (it == this->_users.end())
		throw (std::invalid_argument("The user is not in the channel."));
	this->_users.erase(it);
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
			std::cout << it->getName() << " & " << it->getPassword() << std::endl;
			return (*it);
		}
	}
	throw (std::invalid_argument("Channel not found."));
}

std::map<std::string, std::string>	Channel::splitChannels(const std::string &data) {
	std::map<std::string, std::string>	channels_mdp;
	std::vector<std::string>			channels;
	std::stringstream					storage(data);
	std::string							segment;
	std::vector<std::string>::iterator	iter;

	while (std::getline(storage, segment, ',')) {
		channels.push_back(segment);
	}
	for (iter = channels.begin(); iter != channels.end(); iter++) {
		if (iter->at(0) == '#')
			continue ;
		break ;
	}
	if (iter == channels.begin()) {
		channels_mdp.insert(std::make_pair("invalid", ""));
		return (channels_mdp);
	}
	for (std::vector<std::string>::iterator it = channels.begin(); it != channels.end(); it++) {
		if (it->at(0) != '#')
			break ;
		if (iter == channels.end()) {
			channels_mdp.insert(std::make_pair(*it, ""));
		}
		else {
			channels_mdp.insert(std::make_pair(*it, *iter));
			iter++;
		}
	}
	return (channels_mdp);
}
