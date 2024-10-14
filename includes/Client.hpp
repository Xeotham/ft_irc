#ifndef CLIENT_HPP
# define CLIENT_HPP

#include <iostream>
#include <vector>

class Client
{
	public:
		Client();
		Client(const Client &ref);
		~Client();

		Client	&operator=(const Client &ref);

		void	setFd(int fd);
		void	setIpAdd(std::string add);
		void	setNick(std::string nick);
		void	setUser(std::string user);
		void	setPassword();

		int			getFd() const;
		std::string	getIpAdd() const;
		std::string	getNick() const;
		std::string	getUser() const;
		bool		getPassword() const;
		static Client	&getClientByFd(std::vector<Client> &lst, int fd);
	private:
		int						_fd;
		std::string				_ipAdd;
		std::string				_nick;
		std::string				_user;
		bool					_password;
};




#endif