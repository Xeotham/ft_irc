#include <ACommand.hpp>
#include <PrivMsgCmd.hpp>
#include <JoinCmd.hpp>
#include <PartCmd.hpp>
#include <NickCmd.hpp>
#include <UserCmd.hpp>
#include <WhoCmd.hpp>
#include "KickCmd.hpp"
#include "TopicCmd.hpp"
#include "ModeCmd.hpp"
#include "InviteCmd.hpp"
#include <Bot.hpp>
#include <PingCmd.hpp>

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

ACommand *ACommand::cmdSelector(int fd, UserLst &user_lst, ChannelLst &chan_lst, const std::string &data) {
	size_t						i = 0;
	static const std::string	cmds[] = {MSG, JOIN, PART, NICK, USER, BOT_CMD, WHO, PING, KICK, "TOPIC", "MODE", "INVITE", QUIT,END_OF_ARRAY};
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
		case CMD_WHO:
			return (new WhoCmd(user_lst, chan_lst, new_data));
		case CMD_PING:
			return (new PingCmd(user_lst, chan_lst, new_data));
		case CMD_KICK:
			return (new KickCmd(user_lst, chan_lst, new_data));
		case CMD_TOPIC:
			return (new TopicCmd(user_lst, chan_lst, new_data));
		case CMD_MODE:
			return (new ModeCmd(user_lst, chan_lst, new_data));
		case CMD_INVITE:
			return (new InviteCmd(user_lst, chan_lst, new_data));
		case CMD_QUIT:
			throw (false);
		default:
			throw Error(fd, Client::getClientByFd(user_lst, fd), ERR_UNKNOWNCOMMAND, UNKNOWNCOMMAND_MSG(data));
	}
}

