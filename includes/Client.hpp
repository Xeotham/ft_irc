#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <iostream>
# include <vector>

class Client
{
	private:
		int			_fd;
		std::string	_ipAdd;
		std::string	_nick;
		std::string	_user;
		std::string	_buffer;
		bool		_password;
	public:
		Client();
		virtual ~Client();

		void	setFd(int fd);
		void	setIpAdd(std::string add);
		void	setNick(std::string nick);
		void	setUser(std::string user);
		void	setPassword();
		void	setBuffer(std::string buffer);

		int			getFd() const;
		std::string	getIpAdd() const;
		std::string	getNick() const;
		std::string	getUser() const;
		bool		getPassword() const;
		std::string	getBuffer() const;
		std::string        getSendMsg(const std::string &cmd, const std::string &data, const std::string &dest) const;
		static Client    &getClientByFd(std::vector<Client> &lst, int fd);
};

#endif