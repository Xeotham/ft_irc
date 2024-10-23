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

void NickCmd::execute(int fd)
{
	Client	*user = __nullptr;
	try {
		user = &Client::getClientByFd(*_user_lst, fd);
	}
	catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
		return ;
	}
	for (size_t i = 0; i < _user_lst->size(); i++)
	{
		if ((*_user_lst)[i].getNick() == _data)
		{
			std::string message = "Error : nickname already used.\r\n";
			send(fd, message.c_str(), message.size(), 0);
			std::cout << "Client <" << (*_user_lst)[i].getFd() << "> disconnected." << std::endl;
			close(fd);
			// clearClients(fd);
			return ;
		}
	}
	Messages::sendMsg(fd, _data, *user, NICK);
	user->setNick(_data);
	std::cout << "Client <" << fd << "> set nickname to : " << user->getNick() << std::endl;
}
