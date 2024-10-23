#include <UserCmd.hpp>

UserCmd::UserCmd() {}

UserCmd::UserCmd(const UserCmd &other) : ACommand(other) {}

UserCmd::UserCmd(UserLst &user_lst, ChannelLst &chan_lst, const std::string &data) : ACommand(user_lst, chan_lst, data) {}

UserCmd::~UserCmd() {}

UserCmd	&UserCmd::operator=(const UserCmd &other)
{
	if (this == &other)
		return *this;
	return *this;
}

void UserCmd::execute(int fd) {
	//	USER <username> <hostname> <servername> :<realname>
	Client		&user = Client::getClientByFd(*_user_lst, fd);
	std::string	username = _data;

	username.erase(username.find_first_of(' '));
	Messages::sendMsg(fd, username, user, USER);
	user.setUser(username);
	std::cout << "Client <" << fd << "> set username to : " << user.getUser() << std::endl;
}
