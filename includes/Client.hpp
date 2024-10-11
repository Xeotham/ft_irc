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
		Channel		_current_channel;
	public:
		Client();
		~Client();

		void	setFd(int fd);
		void	setIpAdd(std::string add);
		void	setChannel(const Channel &new_channel);

		Channel	&getChannel();
		int		getFd() const;
};




#endif