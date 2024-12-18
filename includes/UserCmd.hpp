#ifndef USERCMD_HPP
# define USERCMD_HPP

# include "ACommand.hpp"

class UserCmd : public ACommand
{
	public:
		UserCmd(Client &user, UserLst &user_lst, ChannelLst &chan_lst, const std::string &data);
		UserCmd(const UserCmd &other);
		~UserCmd();
	public:
		UserCmd	&operator=(const UserCmd &other);
	public:
		void	execute(int fd);
	private:
		UserCmd();
	private:
		void	checkData(int fd);
		void	checkData(int fd, const std::string &user_name);
};

#endif
