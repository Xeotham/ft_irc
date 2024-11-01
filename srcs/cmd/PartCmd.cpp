#include <PartCmd.hpp>
#include <Channel.hpp>

PartCmd::PartCmd(const PartCmd &other) : ACommand(other) {
	*this = other;
}

PartCmd::PartCmd(Client &user, UserLst &user_lst, ChannelLst &chan_lst, const std::string &data) : ACommand(user, user_lst, chan_lst, data) {}

PartCmd::~PartCmd() {}

PartCmd &PartCmd::operator=(const PartCmd &other) {
	if (this == &other)
		return (*this);
	return (*this);
}

void	PartCmd::exitChannel(Channel &chan, const std::string &msg) {

	if (Channel::isUserInChannel(chan, *_user)) {
		for (UserPtrLst::iterator iter = chan.getUsers().begin(); iter != chan.getUsers().end(); iter++) {
			Messages::sendMsg((*iter)->getFd(), chan.getName() + " " + msg, *_user, PART);
		}
		chan.removeUser(*_user);
		_user->removeChannel(chan);
		if (chan.getUsers().empty())
			Channel::removeChannelFromLst(*_chan_lst, chan);
	}
	else
		throw Error(_user->getFd(), *_user, ERR_NOTONCHANNEL, NOTONCHANNEL_MSG(chan.getName()));
}

void PartCmd::execute(int fd) {
	if (_data.empty())
		throw Error(fd, *_user, ERR_NEEDMOREPARAMS, NEEDMOREPARAMS_MSG("PART"));

	std::vector<std::string>	channels = splitData();
	std::string					msg = _data.substr(_data.find(':') + 1);

	for (std::vector<std::string>::iterator iter = channels.begin(); iter != channels.end(); iter++) {
        try {
        	Channel &chan = Channel::getChannelByName(*_chan_lst, *iter);
			this->exitChannel(chan, msg);
		}
		catch (Error &e) {
			e.sendError();
        }
	}
}

std::vector<std::string>	PartCmd::splitData() {
	std::vector<std::string>			channels;
	std::stringstream					storage(_data.substr(0, _data.find(':') - 1));
	std::string							segment;

	while (std::getline(storage, segment, ',') && !segment.empty()) {
		if (segment.at(0) != '#') {
			Error(_user->getFd(), *_user, ERR_NOSUCHCHANNEL, NOSUCHCHANNEL_MSG(segment)).sendError();
			continue ;
		}
		channels.push_back(segment);
	}
	return (channels);
}
