#ifndef CLIENT_HPP
# define CLIENT_HPP

#include <iostream>
#include <Channel.hpp>

class Channel;

class Client
{
private:
	int			_fd;
	std::string	_ipAdd;
	std::string	_nick;
	std::string	_user;
	int			 _password;
public:
	Client();
	~Client();

	void	setFd(int fd);
	void	setIpAdd(std::string add);
	void	setNick(std::string nick);
	void	setUser(std::string user);
	void	setPassword();

	int		getFd() const;
	std::string	getIpAdd() const;
	std::string	getNick() const;
	std::string	getUser() const;
	int			getPassword() const;
};


class Client
{
	public:
		Client();
		~Client();

		void	setFd(int fd);
		void	setIpAdd(std::string add);
		void	setNick(std::string nick);
		void	setUser(std::string user);
		void	setPassword();
		void	setChannel(Channel &new_channel);

		Channel		*getChannel() const;
		int			getFd() const;
		std::string	getIpAdd() const;
		std::string	getNick() const;
		std::string	getUser() const;
		int			getPassword() const;
	private:
		int			_fd;
		std::string	_ipAdd;
		Channel		*_current_channel;
		std::string	_nick;
		std::string	_user;
		int			 _password;
};




#endif