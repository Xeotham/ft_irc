#include <ACommand.hpp>
#include <PrivMsgCmd.hpp>
#include <JoinCmd.hpp>
#include <PartCmd.hpp>
#include <NickCmd.hpp>
#include <UserCmd.hpp>
#include "KickCmd.hpp"
#include <Bot.hpp>

ACommand::ACommand() : _user_lst(NULL), _chan_lst(NULL) {}

ACommand::ACommand(const ACommand &other) : _user_lst(other._user_lst), _chan_lst(other._chan_lst), _data(other._data) {}

ACommand::ACommand(UserLst &user_lst, ChannelLst &chan_lst, const std::string &data) : _user_lst(&user_lst), _chan_lst(&chan_lst) {
	this->_data = data;
}

ACommand::~ACommand() {}

ACommand &ACommand::operator=(const ACommand &other) {
	if (this == &other)
		return (*this);
	return (*this);
}

ACommand *ACommand::cmdSelector(UserLst &user_lst, ChannelLst &chan_lst, const std::string &data) {
	size_t						i = 0;
	static const std::string	cmds[] = {MSG, JOIN, PART, NICK, USER, "bot", KICK, "QUIT", ""};
	std::string					new_data;

	while (!cmds[i].empty() && data.find(cmds[i].c_str(), 0, cmds[i].size()) == std::string::npos)
		i++;
	new_data = data.substr(cmds[i].size() + 1);
	switch (i) {
		case CMD_PRIVMSG:
			return (new PrivMsgCmd(user_lst, chan_lst, new_data));
		case CMD_JOIN:
			return (new JoinCmd(user_lst, chan_lst, new_data));
		case CMD_PART:
			return (new PartCmd(user_lst, chan_lst, new_data));
		case CMD_NICK:
			return (new NickCmd(user_lst, chan_lst, new_data));
		case CMD_USER:
			return (new UserCmd(user_lst, chan_lst, new_data));
		case CMD_BOT:
			return (new Bot(user_lst, chan_lst, new_data));
		case CMD_KICK:
			return (new KickCmd(user_lst, chan_lst, new_data));
		case CMD_QUIT:
			throw (false);
		default:
			throw (std::invalid_argument("Invalid command"));
	}
}

