#include <PrivMsgCmd.hpp>
#include <Channel.hpp>
#include <Bot.hpp>

PrivMsgCmd::PrivMsgCmd() {}

PrivMsgCmd::PrivMsgCmd(const PrivMsgCmd &other) : ACommand(other) {
	*this = other;
}

PrivMsgCmd::~PrivMsgCmd() {}

PrivMsgCmd &PrivMsgCmd::operator=(const PrivMsgCmd &other)
{
	if (this == &other)
		return *this;
	return *this;
}

void	PrivMsgCmd::sendMsgToChannel(Client &user, const std::string &msg, const std::string &channel, ChannelLst &chan_lst)
{
	Channel &chan = Channel::getChannelByName(chan_lst, channel);
	if (Channel::isUserInChannel(chan, user)) {
		for (UserLst::iterator it = chan.getUsers().begin(); it != chan.getUsers().end(); it++) {
			if (it->getFd() != user.getFd()) {
				std::cout << "Client <" << user.getFd() << "> send message to <" << it->getNick() << "> : " << msg << std::endl;
				Messages::sendMsg(it->getFd(), msg, user, MSG);
			}
		}
	}
}

void	PrivMsgCmd::sendMsgToUser(Client &user, const std::string &msg, const std::string &dest, UserLst &user_lst)
{
    for (UserLst::iterator it = user_lst.begin(); it != user_lst.end(); it++) {
        if (it->getNick() == dest) {
            Messages::sendMsg(it->getFd(), msg, user, MSG);
            return ;
        }
    }
}

void    PrivMsgCmd::sendMsgToBot(int fd, const std::string &data, UserLst &user_lst)
{
    std::string cmd = data;
    cmd.erase(0, data.find(' ') + 2);
    cmd = "bot " + cmd + "\r\n";
	std::cout << cmd << std::endl;
    Bot::botCommand(fd, cmd, user_lst);
}

void	PrivMsgCmd::execute(int fd, const std::string &data, ChannelLst &chan_lst, UserLst &user_lst)
{
	std::string dest = data.substr(0, data.find(' '));
	Client &user = Client::getClientByFd(user_lst, fd);
	if (dest.find_first_of('#') != std::string::npos)
		this->sendMsgToChannel(user, data, dest, chan_lst);
    else if (dest == "Bot") {
    	this->sendMsgToBot(fd, data, user_lst);
    }
	else if (Client::isClientInList(user_lst, dest))
		this->sendMsgToUser(user, data, dest, user_lst);
    else {
        std::string err_message = "Error : user not found.\r\n";
        send(fd, err_message.c_str(), err_message.size(), 0);
    }
}
