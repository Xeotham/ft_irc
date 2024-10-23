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

void	PrivMsgCmd::sendMsgToChannel(Client &user, const std::string &channel)
{
	Channel &chan = Channel::getChannelByName(*_chan_lst, channel);
	if (Channel::isUserInChannel(chan, user)) {
		for (UserLst::iterator it = chan.getUsers().begin(); it != chan.getUsers().end(); it++) {
			if (it->getFd() != user.getFd()) {
				std::cout << "Client <" << user.getFd() << "> send message to <" << it->getNick() << "> : " << _data << std::endl;
				Messages::sendMsg(it->getFd(), _data, user, MSG);
			}
		}
	}
}

void	PrivMsgCmd::sendMsgToUser(Client &user, const std::string &dest)
{
    for (UserLst::iterator it = _user_lst->begin(); it != _user_lst->end(); it++) {
        if (it->getNick() == dest) {
            Messages::sendMsg(it->getFd(), _data, user, MSG);
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
	std::string dest = _data.substr(0, _data.find(' '));
	Client &user = Client::getClientByFd(*_user_lst, fd);
	if (dest.find_first_of('#') != std::string::npos)
		this->sendMsgToChannel(user, dest);
    else if (dest == "Bot")
    	this->sendMsgToBot(fd);
	else if (Client::isClientInList(*_user_lst, dest))
		this->sendMsgToUser(user, dest);
    else {
        std::string err_message = "Error : user not found.\r\n";
        send(fd, err_message.c_str(), err_message.size(), 0);
    }
}
