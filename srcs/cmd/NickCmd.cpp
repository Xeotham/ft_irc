#include <NickCmd.hpp>


NickCmd::NickCmd() {}

NickCmd::NickCmd(const NickCmd &other) : ACommand(other) {}

NickCmd::NickCmd(UserLst &user_lst, ChannelLst &chan_lst, const std::string &data) : ACommand(user_lst, chan_lst, data) {}

NickCmd::~NickCmd() {}

NickCmd &NickCmd::operator=(const NickCmd &other) {
	if (this == &other)
		return *this;
	return *this;
}

void NickCmd::execute(int fd) {
	Client	*user = NULL;
	std::string	nick = _data;

	try {
		user = &Client::getClientByFd(*_user_lst, fd);
	}
	catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
		return ;
	}
	if (_data.empty())
		throw Error(fd, *user, ERR_NONICKNAMEGIVEN, NONICKNAMEGIVEN_MSG);
	if (nick.find_first_of(' ') != std::string::npos)
		nick.erase(nick.find_first_of(' '));
	try {
		Client	&tmp = Client::getClientByNick(*_user_lst, nick);
		static_cast<void> (tmp);
		if (user->getUser().empty()) {
			close(fd);
		}
		throw Error(fd, *user, ERR_NICKNAMEINUSE, NICKNAMEINUSE_MSG(nick));
	}
	catch (Error &e) {
		throw e;
	}
	catch (...) {
		Messages::sendMsg(fd, nick, *user, NICK);
		user->setNick(nick);
		std::cout << "Client <" << fd << "> set nickname to : " << user->getNick() << std::endl;
	}
}
