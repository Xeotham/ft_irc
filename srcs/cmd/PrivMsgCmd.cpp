#include <PrivMsgCmd.hpp>
#include <Channel.hpp>
#include <Bot.hpp>

PrivMsgCmd::PrivMsgCmd() {}

PrivMsgCmd::PrivMsgCmd(const PrivMsgCmd &other) : ACommand(other) {
	*this = other;
}

PrivMsgCmd::~PrivMsgCmd() {}

PrivMsgCmd::PrivMsgCmd(UserLst &user_lst, ChannelLst &chan_lst, const std::string &data) : ACommand(user_lst, chan_lst, data) {}

PrivMsgCmd &PrivMsgCmd::operator=(const PrivMsgCmd &other)
{
	if (this == &other)
		return *this;
	return *this;
}

void	PrivMsgCmd::sendMsgToChannel(Client &user, const std::string &channel, const std::string &msg)
{
	Channel &chan = Channel::getChannelByName(*_chan_lst, channel);

	if (Channel::isUserInChannel(chan, user)) {
		for (UserLst::iterator it = chan.getUsers().begin(); it != chan.getUsers().end(); it++) {
			if (it->getFd() != user.getFd()) {
				std::cout << "Client <" << user.getFd() << "> send message to <" << it->getNick() << "> : " << _data << std::endl;
				Messages::sendMsg(it->getFd(), channel + " :" + msg, user, MSG);
			}
		}
	}
}

void	PrivMsgCmd::sendMsgToUser(Client &user, const std::string &dest, const std::string &msg)
{
    for (UserLst::iterator it = _user_lst->begin(); it != _user_lst->end(); it++) {
        if (it->getNick() == dest) {
            Messages::sendMsg(it->getFd(), dest + " :" + msg, user, MSG);
            return ;
        }
    }
}

void    PrivMsgCmd::sendMsgToBot(int fd)
{
	std::string cmd = _data.substr(5);
	Bot			bot(*_user_lst, *_chan_lst, cmd);

    bot.execute(fd);
}

void	PrivMsgCmd::execute(int fd)
{
	Client						&user = Client::getClientByFd(*_user_lst, fd);
	std::string					msg = _data.substr(_data.find(' ' + 1));
	std::vector<std::string>	dest = splitData(_data.substr(0, _data.find(' ')));

	for (std::vector<std::string>::iterator iter = dest.begin(); iter != dest.end(); iter++) {
		if (iter->find_first_of('#') != std::string::npos)
			this->sendMsgToChannel(user, *iter, msg);
	    else if (*iter == "Bot")
    		this->sendMsgToBot(fd);
		else if (Client::isClientInList(*_user_lst, *iter))
			this->sendMsgToUser(user, *iter, msg);
	    else {
	        std::string err_message = "Error : user not found.\r\n";
	        send(fd, err_message.c_str(), err_message.size(), 0);
	    }
	}


}

std::vector<std::string> PrivMsgCmd::splitData(const std::string &data) {
	std::vector<std::string>	dest;
	std::stringstream			storage(data);
	std::string					segment;

	while (std::getline(storage, segment, ',') && !segment.empty())
		dest.push_back(segment);
	return (dest);
}
