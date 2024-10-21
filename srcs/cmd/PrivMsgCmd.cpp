#include "PrivMsgCmd.h"

PrivMsgCmd::PrivMsgCmd()
{
}

PrivMsgCmd::PrivMsgCmd(const PrivMsgCmd &other)
{
	*this = other;
}

PrivMsgCmd::~PrivMsgCmd()
{
}

PrivMsgCmd &PrivMsgCmd::operator=(const PrivMsgCmd &other)
{
	if (this == &other)
		return *this;
	return *this;
}

void PrivMsgCmd::execute(int fd, const std::string &data, ChannelLst &chan_lst, UserLst &user_lst)
{
	data.erase(0, 8);
	std::string dest = data.substr(0, data.find(' '));
	Client &user = Client::getClientByFd(this->_clients, fd);
	if (dest.find_first_of('#') != std::string::npos) {
		Channel &chan = Channel::getChannelByName(this->_channels, dest);
		if (Channel::isUserInChannel(chan, user)) {
			std::cout << "test" << std::endl;
			for (UserLst::iterator it = chan.getUsers().begin(); it != chan.getUsers().end(); it++) {
				if (it->getFd() != fd) {
					std::cout << "Client <" << fd << "> send message to <" << it->getNick() << "> : " << data << std::endl;
					Messages::sendMsg(it->getFd(), data, user, MSG);
				}
			}
			return ;
		}
	}
	else {
		for (UserLst::iterator it = _clients.begin(); it != _clients.end(); it++) {
			if (it->getNick() == dest) {
				Messages::sendMsg(it->getFd(), data, user, MSG);
				return ;
			}
		}
	}
	std::string err_message = "Error : user not found.\r\n";
	send(fd, err_message.c_str(), err_message.size(), 0);
}
