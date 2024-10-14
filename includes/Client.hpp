#ifndef CLIENT_HPP
# define CLIENT_HPP

#include <iostream>
#include <Channel.hpp>

class Channel;

class Client
{
	public:
		Client();
		~Client();

		void	setFd(int fd);
		void	setIpAdd(std::string add);
		void	setChannel(Channel &new_channel);

		Channel		*getChannel() const;
		int			getFd() const;
	private:
		int			_fd;
		std::string	_ipAdd;
		Channel		*_current_channel;
};




#endif