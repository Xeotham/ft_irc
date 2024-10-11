#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <vector>

class Channel {
public:
	// Default constructor
	Channel();

	// Parameterized constructor
	Channel(const std::string& name);

	// Copy constructor
	Channel(const Channel& other);

	// Copy assignment operator
	Channel& operator=(const Channel& other);

	// Destructor
	~Channel();

	// Getters and Setters
	const std::string& getName() const;
	void setName(const std::string& name);

	// Other member functions
	void addUser(const std::string& user);
	void removeUser(const std::string& user);
	const std::vector<std::string>& getUsers() const;

private:
	std::string name;
	std::vector<std::string> users;
};

#endif // CHANNEL_HPP