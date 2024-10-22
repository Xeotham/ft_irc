#include <UserCmd.hpp>

UserCmd::UserCmd() {}

UserCmd::UserCmd(const UserCmd &other)
{
	*this = other;
}

UserCmd::~UserCmd() {}

UserCmd	&UserCmd::operator=(const UserCmd &other)
{
	if (this == &other)
		return *this;
	return *this;
}

void UserCmd::execute(int fd, const std::string &data, ChannelLst &chan_lst, UserLst &user_lst) {
	//	USER <username> <hostname> <servername> :<realname>
	(void)chan_lst;
	Client	&user = Client::getClientByFd(user_lst, fd);

	Messages::sendMsg(fd, data, user, USER);
	user.setUser(data);
	std::cout << "Client <" << fd << "> set username to : " << it->getUser() << std::endl;
}
