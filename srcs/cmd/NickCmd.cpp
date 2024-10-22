#include <NickCmd.hpp>


NickCmd::NickCmd() {}

NickCmd::NickCmd(const NickCmd &other) : ACommand(other) {
	*this = other;
}

NickCmd::~NickCmd() {}

NickCmd &NickCmd::operator=(const NickCmd &other) {
	if (this == &other)
		return *this;
	return *this;
}

void NickCmd::execute(int fd, const std::string &data, ChannelLst &chan_lst, UserLst &user_lst)
{
	(void)chan_lst;
	try {
		Client	&user = Client::getClientByFd(user_lst, fd);
	}
	catch (std::exception &e) {

		std::cerr << e.what() << std::endl;
	}

	for (size_t i = 0; i < user_lst.size(); i++)
	{
		if (user_lst[i].getNick() == data)
		{
			std::string message = "Error : nickname already used.\r\n";
			send(fd, message.c_str(), message.size(), 0);
			std::cout << "Client <" << user_lst[i].getFd() << "> disconnected." << std::endl;
			close(fd);
			// clearClients(fd);
			return ;
		}
	}
	Messages::sendMsg(fd, data, user, NICK);
	user.setNick(data);
	std::cout << "Client <" << fd << "> set nickname to : " << user.getNick() << std::endl;
}
