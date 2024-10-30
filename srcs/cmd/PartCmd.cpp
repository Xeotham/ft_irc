#include <PartCmd.hpp>
#include <Channel.hpp>

PartCmd::PartCmd(const PartCmd &other) : ACommand(other) {
	*this = other;
}

PartCmd::PartCmd(UserLst &user_lst, ChannelLst &chan_lst, const std::string &data) : ACommand(user_lst, chan_lst, data) {}

PartCmd::~PartCmd() {}

PartCmd &PartCmd::operator=(const PartCmd &other) {
	if (this == &other)
		return (*this);
	return (*this);
}

void	PartCmd::exitChannel(Client	&user, Channel &chan, const std::string &msg) {

	if (Channel::isUserInChannel(chan, user)) {
		for (UserLst::iterator iter = chan.getUsers().begin(); iter != chan.getUsers().end(); iter++) {
			Messages::sendMsg(iter->getFd(), chan.getName() + " " + msg, user, PART);
		}
		chan.removeUser(user);
		user.removeChannel(chan);
		if (chan.getUsers().empty())
			Channel::removeChannelFromLst(*_chan_lst, chan);
	}
	else
		throw (std::invalid_argument("The user is not in the channel."));
}

void PartCmd::execute(int fd) {
	Client						&user = Client::getClientByFd(*_user_lst, fd);

	if (_data.empty())
		throw Error(fd, user, ERR_NEEDMOREPARAMS, NEEDMOREPARAMS_MSG("PART"));

	std::vector<std::string>	channels = splitData(fd);
	std::string					msg = _data.substr(_data.find(':') + 1);

	for (std::vector<std::string>::iterator iter = channels.begin(); iter != channels.end(); iter++) {
        try {
        	Channel &chan = Channel::getChannelByName(*_chan_lst, *iter);
			this->exitChannel(user, chan, msg);
		}
		catch (std::exception &e) {
			throw Error(fd, user, ERR_NOTONCHANNEL, NOTONCHANNEL_MSG(*iter));
        }
	}
}

std::vector<std::string>	PartCmd::splitData(int fd) {
	std::vector<std::string>			channels;
	std::stringstream					storage(_data.substr(0, _data.find(':') - 1));
	std::string							segment;

	while (std::getline(storage, segment, ',') && !segment.empty())
		channels.push_back(segment);
	for (std::vector<std::string>::iterator iter = channels.begin(); iter != channels.end(); iter++) {
		if (iter->at(0) != '#')
			throw Error(fd,Client::getClientByFd(*_user_lst, fd), ERR_NOSUCHCHANNEL, NOSUCHCHANNEL_MSG(*iter));
	}
	return (channels);
}
