#ifndef CHANNEL_HPP
# define CHANNEL_HPP


#include <string>
#include <vector>
#include <Client.hpp>

class Client;

class Channel {
	public:
		Channel(const std::string& name);
		Channel(const Channel& other);
		~Channel();
    public:
		Channel& operator=(const Channel& other);
	public:
		void						setName(const std::string& name);
		void						addUser(Client& user);
		void						removeUser(const Client& user);
		const std::vector<Client *>	&getUsers() const;
		const std::string			&getName() const;
	public:
		// static std::vector<Channel>::const_iterator	&getChannelByName(std::vector<Channel> lst, const std::string &name);
	private:
		Channel();
	private:
		std::string			_name;
		std::vector<Client *> _users;
};

#endif // CHANNEL_HPP