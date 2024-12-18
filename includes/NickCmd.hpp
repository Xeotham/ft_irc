#ifndef NICKCMD_HPP
# define NICKCMD_HPP

#include "ACommand.hpp"

class NickCmd : public ACommand
{
	public:
		NickCmd(Client &user, UserLst &user_lst, ChannelLst &chan_lst, const std::string &data);
		NickCmd(const NickCmd &other);
		~NickCmd();
	public:
		NickCmd &operator=(const NickCmd &other);
	public:
		void	execute(int fd);
	private:
		NickCmd();
	private:
		void	sendToEveryone();
};

#endif
