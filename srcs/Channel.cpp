#include "Channel.hpp"
#include <algorithm>

// Default constructor
Channel::Channel() : name("") {}

// Parameterized constructor
Channel::Channel(const std::string& name) : name(name) {}

// Copy constructor
Channel::Channel(const Channel& other) : name(other.name), users(other.users) {}

// Copy assignment operator
Channel& Channel::operator=(const Channel& other) {
	if (this != &other) {
		name = other.name;
		users = other.users;
	}
	return *this;
}

// Destructor
Channel::~Channel() {}

// Getters and Setters
const std::string& Channel::getName() const {
	return name;
}

void Channel::setName(const std::string& name) {
	this->name = name;
}

// Other member functions
void Channel::addUser(const std::string& user) {
	users.push_back(user);
}

void Channel::removeUser(const std::string& user) {
	users.erase(std::remove(users.begin(), users.end(), user), users.end());
}

const std::vector<std::string>& Channel::getUsers() const {
	return users;
}