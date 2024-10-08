#ifndef CLIENT_HPP
# define CLIENT_HPP

#include <iostream>

class Client
{
	private:
		int	_fd;
		std::string	_ipAdd;
	public:
		Client();
		~Client();

		void	setFd(int fd);
		void	setIpAdd(std::string add);

		int		getFd() const;
};




#endif