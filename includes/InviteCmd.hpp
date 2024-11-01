#ifndef INVITE_HPP
# define INVITE_HPP

#include "ACommand.hpp"
#include "Client.hpp"

class InviteCmd : public ACommand 
{
	private:
		InviteCmd();
	public:
		InviteCmd(Client &user, UserLst &user_lst, ChannelLst &chan_lst, const std::string &data);
		virtual ~InviteCmd();
	public:
		void execute(int fd);
};

#endif