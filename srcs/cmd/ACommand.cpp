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
#include <InviteCmd.hpp>
#include <BotCmd.hpp>
#include <PingCmd.hpp>
#include <ListCmd.hpp>
#include <NamesCmd.hpp>

ACommand::ACommand() : _user(NULL), _user_lst(NULL), _chan_lst(NULL) {}

ACommand::ACommand(const ACommand &other) :_user(other._user), _user_lst(other._user_lst), _chan_lst(other._chan_lst), _data(other._data) {}

ACommand::ACommand(Client &user, UserLst &user_lst, ChannelLst &chan_lst, const std::string &data) : _user(&user), _user_lst(&user_lst), _chan_lst(&chan_lst) {
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
	static const std::string	cmds[] = {MSG, JOIN, PART, NICK, USER, BOT_CMD, WHO, PING, KICK, TOPIC, MODE , INVITE, CAP, NAMES, LIST, QUIT,END_OF_ARRAY};
	std::string					new_data;
	Client						&user = Client::getClientByFd(user_lst, fd);

	while (!cmds[i].empty() && data.find(cmds[i].c_str(), 0, cmds[i].size()) == std::string::npos)
		i++;
	new_data = data.substr(cmds[i].size());
	if (!new_data.empty() && new_data.at(0) == ' ')
		new_data.erase(0, 1);
	if ((user.getUser().empty() || user.getNick().empty() || !user.getIsSet()) && (i != CMD_USER && i != CMD_NICK))
		throw (true);
	switch (i) {
		case CMD_PRIVMSG:
			return (new PrivMsgCmd(user, user_lst, chan_lst, new_data));
		case CMD_JOIN:
			return (new JoinCmd(user, user_lst, chan_lst, new_data));
		case CMD_PART:
			return (new PartCmd(user, user_lst, chan_lst, new_data));
		case CMD_NICK:
			return (new NickCmd(user, user_lst, chan_lst, new_data));
		case CMD_USER:
			return (new UserCmd(user, user_lst, chan_lst, new_data));
		case CMD_BOT:
			return (new BotCmd(user, user_lst, chan_lst, new_data));
		case CMD_WHO:
			return (new WhoCmd(user, user_lst, chan_lst, new_data));
		case CMD_PING:
			return (new PingCmd(user, user_lst, chan_lst, new_data));
		case CMD_KICK:
			return (new KickCmd(user, user_lst, chan_lst, new_data));
		case CMD_TOPIC:
			return (new TopicCmd(user, user_lst, chan_lst, new_data));
		case CMD_MODE:
			return (new ModeCmd(user, user_lst, chan_lst, new_data));
		case CMD_INVITE:
			return (new InviteCmd(user, user_lst, chan_lst, new_data));
		case CMD_NAMES:
			return (new NamesCmd(user, user_lst, chan_lst, new_data));
		case CMD_LIST:
			return (new ListCmd(user, user_lst, chan_lst, new_data));
		case CMD_CAP:
			throw (true);
		case CMD_QUIT:
			throw (false);
		default:
			throw Error(fd, user, ERR_UNKNOWNCOMMAND, UNKNOWNCOMMAND_MSG(data));
	}
}

