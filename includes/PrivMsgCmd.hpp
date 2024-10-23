#ifndef PRIVMSGCMD_HPP
# define PRIVMSGCMD_HPP

# include "ACommand.hpp"

class PrivMsgCmd : public ACommand {
	public:
		PrivMsgCmd(const PrivMsgCmd &other);
		PrivMsgCmd(UserLst &user_lst, ChannelLst &chan_lst, const std::string &data);
		virtual ~PrivMsgCmd();
	public:
		PrivMsgCmd &operator=(const PrivMsgCmd &other);
	public:
		void execute(int fd);
	private:
		PrivMsgCmd();
	private:
		void						sendMsgToChannel(Client &user, const std::string &channel, const std::string &msg);
        void						sendMsgToUser(Client &user, const std::string &dest);
		void						sendMsgToBot(int fd);
		std::vector<std::string>	splitData(const std::string &data);
};



#endif
