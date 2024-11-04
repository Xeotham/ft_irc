#include <ListCmd.hpp>

ListCmd::ListCmd(Client &user, UserLst &user_lst, ChannelLst &chan_lst, const std::string &data) : ACommand(user, user_lst, chan_lst, data){}

ListCmd::ListCmd(const ListCmd &other) : ACommand(other){}

ListCmd::~ListCmd(){}

ListCmd &ListCmd::operator=(const ListCmd &other)
{
	if (this == &other)
		return (*this);
	return (*this);
}

void	ListCmd::listChannels(std::vector<Channel> &lst) {
	Messages::sendServMsg(_user->getFd(), _user->getNick() + " Channel :Users  Name", LISTSTART);
	for (std::vector<Channel>::iterator it = lst.begin(); it != lst.end(); it++) {
		std::string chan_size;
		std::stringstream out;
		out << it->getUsers().size();
		chan_size = out.str();
		std::string	msg = _user->getNick() + " " + it->getName() + " " + chan_size + " :" + it->getTopic();
		Messages::sendServMsg(_user->getFd(), msg, LIST_RPL);
	}
	Messages::sendServMsg(_user->getFd(), _user->getNick() + " :End of /LIST", LISTEND);
}

void ListCmd::execute(int fd) {
	static_cast<void>(fd);

	if (_data.empty())
		listChannels(*_chan_lst);
	else {
		std::vector<Channel>	channels = splitData();

		listChannels(channels);
	}
}

std::vector<Channel>	ListCmd::splitData() {
	std::vector<Channel>	channels;
	std::stringstream		storage(_data);
	std::string				segment;

	while (std::getline(storage, segment, ',') && !segment.empty()) {
		if (segment.at(0) != '#') {
			Error(_user->getFd(), *_user, ERR_NOSUCHCHANNEL, NOSUCHCHANNEL_MSG(segment)).sendError();
			continue ;
		}
		try {
			Channel	&chan = Channel::getChannelByName(*_chan_lst, segment);
			channels.push_back(chan);
		}
		catch (...) {
			Error(_user->getFd(), *_user, ERR_NOSUCHCHANNEL, NOSUCHCHANNEL_MSG(segment)).sendError();
		}
	}
	return (channels);
}
