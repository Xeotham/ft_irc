#ifndef MODE_HPP
# define MODE_HPP

#include "ACommand.hpp"
#include "Client.hpp"

class ModeCmd : public ACommand 
{
	private:
		void	kMode(Channel *target_channel, const bool enable, const std::string arg);
		void	oMode(Channel *target_channel, const bool enable, std::string arg, int fd, Client &sender);
		void	lMode(Channel *target_channel, const bool enable, const std::string arg);
		ModeCmd();
	public:
		ModeCmd(UserLst &user_lst, ChannelLst &chan_lst, const std::string &data);
		virtual ~ModeCmd();
	public:
		void execute(int fd);
};

#endif