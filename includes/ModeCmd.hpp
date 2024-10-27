#ifndef MODE_HPP
# define MODE_HPP

#include "ACommand.hpp"
#include "Client.hpp"

class ModeCmd : public ACommand 
{
	private:
		ModeCmd();
	public:
		ModeCmd(UserLst &user_lst, ChannelLst &chan_lst, const std::string &data);
		virtual ~ModeCmd();
	public:
		void execute(int fd);
};

#endif