#ifndef ACOMMAND_HPP
# define ACOMMAND_HPP

# include "Params.hpp"

class ACommand {
	public:
		ACommand(const ACommand &other);
		ACommand(Client &user, UserLst &user_lst, ChannelLst &chan_lst, const std::string &data);
		virtual ~ACommand();
	public:
		ACommand &operator=(const ACommand &other);
	public:
		static ACommand	*cmdSelector(int fd, UserLst &user_lst, ChannelLst &chan_lst, const std::string &data);
		virtual void	execute(int fd) = 0;
	protected:
		Client		*_user;
		UserLst		*_user_lst;
		ChannelLst	*_chan_lst;
		std::string	_data;
	protected:
		ACommand();
	private:

	private:
		enum e_cmd {
			CMD_PRIVMSG,
			CMD_JOIN,
			CMD_PART,
			CMD_NICK,
			CMD_USER,
			CMD_BOT,
			CMD_WHO,
			CMD_PING,
			CMD_KICK,
			CMD_TOPIC,
			CMD_MODE,
			CMD_INVITE,
			CMD_CAP,
			CMD_NAMES,
			CMD_LIST,
			CMD_QUIT,
		};
};



#endif
