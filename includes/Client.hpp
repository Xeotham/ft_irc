#ifndef CLIENT_HPP
# define CLIENT_HPP

#include <Params.hpp>

class Client
{
	public:
		Client();
		Client(const Client &ref);
		Client(const std::string &nick, const std::string &user);
		~Client();

		Client	&operator=(const Client &ref);

		void	setFd(int fd);
		void	setIpAdd(const std::string &add);
		void	setNick(const std::string &nick);
		void	setUser(const std::string &user);
		void	setPassword();
		void	setBuffer(const std::string &buffer);
		void	addChannel(const std::string &channel);
		void	removeChannel(const std::string &channel);

		int							getFd() const;
		std::string					getIpAdd() const;
		std::string					getNick() const;
		std::string					getUser() const;
		std::string					getBuffer() const;
		bool						getPassword() const;
		std::vector<std::string>	&getChannels();
		static Client				&getClientByFd(std::vector<Client> &lst, int fd);
	private:
		int							_fd;
		std::string					_ipAdd;
		std::string					_nick;
		std::string					_user;
		std::string					_buffer;
		bool						_password;
		std::vector<std::string>	_channels;
};

#endif