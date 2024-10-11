#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include <string>
#include <vector>
#include <Client.hpp>

class Channel {
	public:
		Channel(const std::string& name);
		~Channel();
    public:
		Channel& operator=(const Channel& other);
	public:
		const std::string& getName() const;
		void setName(const std::string& name);
		void addUser(const Client& user);
		void removeUser(const Client& user);
		const std::vector<Client>& getUsers() const;
	private:
		Channel(const Channel& other);
		Channel();
	private:
		std::string _name;
		std::vector<Client> _users;
};

#endif // CHANNEL_HPP