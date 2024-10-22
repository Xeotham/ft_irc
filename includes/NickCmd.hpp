#ifndef NICKCMD_HPP
# define NICKCMD_HPP

#include "ACommand.hpp"

class NickCmd : public ACommand
{
	public:
		NickCmd();
		NickCmd(const NickCmd &other);
		~NickCmd();
	public:
		NickCmd &operator=(const NickCmd &other);
	public:
		void	execute(int fd, const std::string &data, ChannelLst &chan_lst, UserLst &user_lst);

};

#endif //NICKCMD_H
