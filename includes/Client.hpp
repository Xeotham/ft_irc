#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Params.hpp"
# include "Channel.hpp"

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
		void	setRealname(const std::string &realname);
		void	setPassword();
		void	setBuffer(const std::string &buffer);
		void	addChannel(const Channel channel);
		void	removeChannel(const Channel &channel);

		int							getFd() const;
		std::string					getIpAdd() const;
		std::string					getNick() const;
		std::string					getUser() const;
		std::string					getRealname() const;
		std::string					getBuffer() const;
		bool						getPassword() const;
		ChannelLst					&getChannels();
		static Client				&getClientByFd(UserLst &lst, int fd);
		static Client				&getClientByNick(UserLst &lst, const std::string &nick);
		static Client				&getClientByUser(UserLst &lst, const std::string &user);
		static bool					isClientInList(UserLst &lst, const std::string &nick);
	private:
		int			_fd;
		std::string	_ipAdd;
		std::string	_nick;
		std::string	_user;
		std::string	_realname;
		std::string	_buffer;
		bool		_password;
		ChannelLst	_channels;
};

#endif