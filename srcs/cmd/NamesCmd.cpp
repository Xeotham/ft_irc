#include <NamesCmd.hpp>

NamesCmd::NamesCmd(UserLst &user_lst, ChannelLst &chan_lst, const std::string &data) : ACommand(user_lst, chan_lst, data){}

NamesCmd::NamesCmd(const NamesCmd &other) : ACommand(other){}

NamesCmd::~NamesCmd(){}

NamesCmd &NamesCmd::operator=(const NamesCmd &other)
{
	if (this == &other)
		return (*this);
	return (*this);
}

void	NamesCmd::sendNames(Client &user, Channel &chan) {
	std::string	names = user.getNick() + " = " + chan.getName() + " :";

	for (UserLst::iterator it = chan.getUsers().begin(); it != chan.getUsers().end(); it++) {
		names += it->getNick();
		if (it + 1 != chan.getUsers().end())
			names += ' ';
	}
	Messages::sendServMsg(user.getFd(), names, NAMREPLY);
	Messages::sendServMsg(user.getFd(), user.getNick() + " " + chan.getName() + " :End of /NAMES list.", ENDOFNAMES);
}

void	NamesCmd::sendAllNames(Client &user, ChannelLst &chan_lst) {
	for (ChannelLst::iterator it = chan_lst.begin(); it != chan_lst.end(); it++) {
		this->sendNames(user, *it);
	}
}

void	NamesCmd::sendAllNames(Client &user, std::vector<std::string> &chan_lst) {
	for (std::vector<std::string>::iterator it = chan_lst.begin(); it != chan_lst.end(); it++) {
		this->sendNames(user, Channel::getChannelByName(*_chan_lst, *it));
	}
}

void NamesCmd::execute(int fd) {
	std::vector<std::string>	channels = this->splitData();
	Client						&user = Client::getClientByFd(*_user_lst, fd);

	if (channels.empty())
		this->sendAllNames(user, *_chan_lst);
	else
		this->sendAllNames(user, channels);
}

std::vector<std::string>	NamesCmd::splitData() {
	std::vector<std::string>			channels;
	std::stringstream					storage(_data);
	std::string							segment;

	while (std::getline(storage, segment, ',') && !segment.empty())
		channels.push_back(segment);
	return (channels);
}