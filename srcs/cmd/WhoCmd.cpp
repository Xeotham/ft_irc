#include <WhoCmd.hpp>

WhoCmd::WhoCmd() {}

WhoCmd::WhoCmd(UserLst &user_lst, ChannelLst &chan_lst, const std::string &data) : ACommand(user_lst, chan_lst, data) {}

WhoCmd::WhoCmd(const WhoCmd &other) : ACommand(other) {}

WhoCmd::~WhoCmd() {}

WhoCmd	&WhoCmd::operator=(const WhoCmd &other)
{
	if (this == &other)
		return (*this);
	return (*this);
}

void	WhoCmd::sendData(int fd, Client &user, Channel &chan) {
	for (UserLst::iterator it = chan.getUsers().begin(); it != chan.getUsers().end(); it++) {
		std::string	data = user.getNick() + " " + chan.getName() + " " + it->getUser() + " localhost " + SERVER_NAME + " " + it->getNick() + " :0 " + it->getRealname();
		Messages::sendServMsg(fd, data, WHOREPLY);
	}
}

void	WhoCmd::sendData(int fd, Client &user, Client &target) {
	std::string	data = user.getNick() + " " + target.getUser() + " localhost " + SERVER_NAME + " " + target.getNick() + " :0 " + user.getRealname();
	Messages::sendServMsg(fd, data, WHOREPLY);
}

void WhoCmd::execute(int fd) {
	try {
		Client &user = Client::getClientByFd(*_user_lst, fd);
		if (_data.at(0) == '#')
			this->sendData(fd, user, Channel::getChannelByName(*_chan_lst, _data));
		else
			this->sendData(fd, user, Client::getClientByUser(*_user_lst, _data));
		Messages::sendServMsg(fd, user.getNick() + " " + _data + " :End of /WHO list.", ENDOFWHO);
	}
	catch (std::exception &e) {
		std::string err_msg;
		if (std::string(e.what()) == "Channel not found.")
			err_msg = "Error : " + _data + " is not a valid channel name.";
		else
			err_msg = "Error : " + _data + " is not a valid user name.";
		Messages::sendServMsg(fd, err_msg, "403");
	}
}