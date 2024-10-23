#ifndef USERCMD_HPP
# define USERCMD_HPP

# include "ACommand.hpp"

class UserCmd : public ACommand
{
	public:
		UserCmd(UserLst &user_lst, ChannelLst &chan_lst, const std::string &data);
		UserCmd(const UserCmd &other);
		~UserCmd();
	public:
		UserCmd	&operator=(const UserCmd &other);
	public:
		void	execute(int fd);
	private:
		UserCmd();
};

#endif
