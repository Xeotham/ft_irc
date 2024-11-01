#ifndef PRIVMSGCMD_HPP
# define PRIVMSGCMD_HPP

# include "ACommand.hpp"

class PrivMsgCmd : public ACommand {
	public:
		PrivMsgCmd(const PrivMsgCmd &other);
		PrivMsgCmd(Client &user, UserLst &user_lst, ChannelLst &chan_lst, const std::string &data);
		virtual ~PrivMsgCmd();
	public:
		PrivMsgCmd &operator=(const PrivMsgCmd &other);
	public:
		void execute(int fd);
	private:
		PrivMsgCmd();
	private:
		void						sendMsgToChannel(const std::string &channel, const std::string &msg);
        void						sendMsgToUser(const std::string &dest, const std::string &msg);
		void						sendMsgToBot(int fd);
		std::vector<std::string>	splitData(const std::string &data);
};



#endif
