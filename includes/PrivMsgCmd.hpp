#ifndef PRIVMSGCMD_HPP
# define PRIVMSGCMD_HPP

# include <ACommand.hpp>

class PrivMsgCmd : public ACommand {
	public:
		PrivMsgCmd();
		PrivMsgCmd(const PrivMsgCmd &other);
		virtual ~PrivMsgCmd();
	public:
		PrivMsgCmd &operator=(const PrivMsgCmd &other);
	public:
		void execute(int fd, const std::string &data, ChannelLst &chan_lst, UserLst &user_lst);

};



#endif