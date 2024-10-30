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
	this->checkData(fd);

	Client		&user = Client::getClientByFd(*_user_lst, fd);
	std::string	username = _data.substr(0, _data.find(' '));
	std::string	realname = _data.substr(_data.find_last_of(':', 0) + 1);

	this->checkData(fd, username);
	if (!user.getUser().empty())
		Messages::sendMsg(fd, username, user, USER);
	user.setUser(username);
	user.setRealname(realname);
	std::cout << "Client <" << fd << "> set username to : " << user.getUser() << std::endl;
}

void UserCmd::checkData(int fd) {
	std::vector<std::string>	split;
	std::stringstream			storage(_data);
	std::string					segment;

	std::cout << _data << std::endl;
	while (std::getline(storage, segment, ' ') && !segment.empty())
		split.push_back(segment);
	if (split.size() != 4 && (split.end() - 1)->at(0) != ':')
		throw Error(fd, Client::getClientByFd(*_user_lst, fd), ERR_NEEDMOREPARAMS, NEEDMOREPARAMS_MSG("USER"));
}

void	UserCmd::checkData(int fd, const std::string &user_name) {
	for (UserLst::iterator it = _user_lst->begin(); it != _user_lst->end(); it++) {
		if (it->getFd() == fd && !it->getUser().empty() && it->getUser() != user_name)
			throw Error(fd, Client::getClientByFd(*_user_lst, fd), ERR_ALREADYREGISTRED, ALREADYREGISTRED_MSG);
		if (it->getFd() == fd && (it->getUser() == user_name || it->getUser().empty()))
			return ;
	}
}