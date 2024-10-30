#include <Channel.hpp>
#include <algorithm>
#include <Client.hpp>
#include <sstream>

Channel::Channel() {
	this->_name = "";
	this->_topic = "";
	this->_user_limit = 0;
}

Channel::Channel(const std::string &name) {
	if (name.at(0) != '#' || name.size() > 50 || name.size() < 2
		|| name.find_first_of(" ,\007:") != std::string::npos)
		throw (std::invalid_argument("Invalid channel name."));
	this->_name = name;
	this->_topic = "";
	this->_user_limit = 0;
}

Channel::Channel(const std::string &name, const std::string &pwd) {
	if (name.at(0) != '#' || name.size() > 50 || name.size() < 2
		|| name.find_first_of(" ,\007:") != std::string::npos)
		throw (std::invalid_argument("Invalid channel name."));
	this->_name = name;
	this->_password = pwd;
	this->_topic = "";
	this->_user_limit = 0;
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
	this->_operators = other._operators;
	this->_topic = other._topic;
	this->_modes = other._modes;
	this->_user_limit = other._user_limit;
	this->_invited_users = other._invited_users;
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
void Channel::addUser(Client user) {
	for (UserLst::iterator it = this->_users.begin(); it != this->_users.end(); it++) {
		if (it->getFd() == user.getFd())
			throw (std::invalid_argument("The user is already in the " + this->getName() + " channel."));
	}
	this->_users.push_back(user);
}

void	Channel::addOperator(Client user){
	for (UserLst::iterator it = this->_operators.begin(); it != this->_operators.end(); it++) {
		if (it->getFd() == user.getFd())
			throw (std::invalid_argument("The user: " + this->getName() + "is already an operator in the channel."));
	}
	this->_operators.push_back(user);
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

void Channel::removeOperator(const Client &user) {
	for (UserLst::iterator it = this->_operators.begin(); it != this->_operators.end(); it++) {
		if (it->getFd() == user.getFd()) {
			this->_operators.erase(it);
			return ;
		}
	}
	throw (std::invalid_argument("The user is not an operator in the channel."));
}

UserLst&	Channel::getOperators(){
	return (this->_operators);
}

std::vector<Client> &Channel::getUsers() {
	return (this->_users);
}

const std::string &Channel::getPassword() const {
	return (this->_password);
}

Channel &Channel::getChannelByName(ChannelLst &lst, const std::string &name) {
	for (ChannelLst::iterator it = lst.begin(); it != lst.end(); it++) {
		if (it->getName() == name) {
			return (*it);
		}
	}
	throw (std::invalid_argument("Channel not found."));
}

Channel *Channel::getChannelByNamePt(ChannelLst &lst, const std::string &name) {
	for (ChannelLst::iterator it = lst.begin(); it != lst.end(); it++) {
		if (it->getName() == name) {
			return (&(*it));
		}
	}
	return (NULL);
}

bool	Channel::isUserInChannel(Channel &channel, const Client &user) {
	for (UserLst::const_iterator it = channel.getUsers().begin(); it != channel.getUsers().end(); it++) {
		if (it->getFd() == user.getFd())
			return (true);
	}
	return (false);
}

bool	Channel::isOperatorInChannel(Channel &channel, const Client &user){
	for (UserLst::const_iterator it = channel.getOperators().begin(); it != channel.getOperators().end(); it++) {
		if (it->getFd() == user.getFd())
			return (true);
	}
	return (false);
}

const std::string &Channel::getTopic() const {
	return (this->_topic);
}

void Channel::setTopic(const std::string &topic) {
	this->_topic = topic;
}

void	Channel::setMode(const char mode, const bool enable){
	if (enable)
		_modes.insert(mode);
	else
		_modes.erase(mode);
}

bool	Channel::isModeSet(const char mode) const{
	if (_modes.find(mode) != _modes.end())
		return (true);
	else
		return (false);
}

const std::string	Channel::getModes() const{
	std::string modes;
	for (std::set<char>::iterator it = _modes.begin(); it != _modes.end(); ++it) {
		modes += *it;
	}
	return (modes);
}

bool	Channel::isExistingMode(const char c) const{
	const std::string	existing_modes = "itkol+-";
	if (existing_modes.find(c) != std::string::npos)
		return (true);
	else
		return (false);
}

unsigned int	Channel::getUserLimit() const {
	return (this->_user_limit);
}

void Channel::setUserLimit(unsigned int limit) {
	this->_user_limit = limit;
}

void Channel::setPassword(const std::string &password) {
	this->_password = password;
}

bool	Channel::isInvitedUserInChannel(Channel &channel, const Client &user) {
	for (UserLst::const_iterator it = channel._invited_users.begin(); it != channel._invited_users.end(); it++) {
		if (it->getFd() == user.getFd())
			return (true);
	}
	return (false);
}

void Channel::addInvitedUser(Client user) {
	for (UserLst::iterator it = this->_invited_users.begin(); it != this->_invited_users.end(); it++) {
		if (it->getFd() == user.getFd())
			throw (std::invalid_argument("The user is already invited to the " + this->getName() + " channel."));
	}
	this->_invited_users.push_back(user);
}
