#ifndef KICK_HPP
# define KICK_HPP

#include "ACommand.hpp"
#include "Client.hpp"

class KickCmd : public ACommand 
{
	private:
		KickCmd();
	public:
		KickCmd(UserLst &user_lst, ChannelLst &chan_lst, const std::string &data);
		virtual ~KickCmd();
	public:
		void execute(int fd);
};

#endif