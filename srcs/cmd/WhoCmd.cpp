#include <WhoCmd.hpp>

WhoCmd::WhoCmd() {}

WhoCmd::WhoCmd(Client &user, UserLst &user_lst, ChannelLst &chan_lst, const std::string &data) : ACommand(user, user_lst, chan_lst, data) {}

WhoCmd::WhoCmd(const WhoCmd &other) : ACommand(other) {}

WhoCmd::~WhoCmd() {}

WhoCmd	&WhoCmd::operator=(const WhoCmd &other)
{
	if (this == &other)
		return (*this);
	return (*this);
}

void	WhoCmd::sendData(int fd, Channel &chan) {
	for (UserLst::iterator it = chan.getUsers().begin(); it != chan.getUsers().end(); it++) {
		std::string	data = _user->getNick() + " " + chan.getName() + " " + it->getUser() + " localhost " + SERVER_NAME + " " + it->getNick() + " :0 " + it->getRealname();
		Messages::sendServMsg(fd, data, WHOREPLY);
	}
}

void	WhoCmd::sendData(int fd, Client &target) {
	std::string	data = _user->getNick() + " " + target.getUser() + " localhost " + SERVER_NAME + " " + target.getNick() + " :0 " + target.getRealname();
	Messages::sendServMsg(fd, data, WHOREPLY);
}

void WhoCmd::execute(int fd) {
	try {
		if (_data.at(0) == '#')
			this->sendData(fd, Channel::getChannelByName(*_chan_lst, _data));
		else
			this->sendData(fd, Client::getClientByUser(*_user_lst, _data));
		Messages::sendServMsg(fd, _user->getNick() + " " + _data + " :End of /WHO list.", ENDOFWHO);
	}
	catch (std::exception &e) {
		Error(_user->getFd(), *_user, ERR_NOSUCHNICK, NOSUCHNICK_MSG(_data)).sendError();
	}
}