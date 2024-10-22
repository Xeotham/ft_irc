#ifndef USERCMD_HPP
# define USERCMD_HPP

# include "ACommand.hpp"


class UserCmd : public ACommand
{
	public:
		UserCmd();
		UserCmd(const UserCmd &other);
		~UserCmd();
	public:
		UserCmd	&operator=(const UserCmd &other);
	public:
		void	execute(int fd, const std::string &data, ChannelLst &chan_lst, UserLst &user_lst);

};

#endif
