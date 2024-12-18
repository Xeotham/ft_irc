#include <JoinCmd.hpp>
#include <Channel.hpp>
#include <NamesCmd.hpp>
#include <PartCmd.hpp>

#include "TopicCmd.hpp"

JoinCmd::JoinCmd() : ACommand(){}

JoinCmd::JoinCmd(const JoinCmd &other) : ACommand(other) {
	*this = other;
}

JoinCmd::JoinCmd(Client &user, UserLst &user_lst, ChannelLst &chan_lst, const std::string &data) : ACommand(user, user_lst, chan_lst, data) {}

JoinCmd::~JoinCmd() {}

JoinCmd &JoinCmd::operator=(const JoinCmd &other) {
	if (this == &other)
		return (*this);
	return (*this);
}

void	JoinCmd::joinChannel(const std::pair<std::string, std::string> &data) {
	try {
		Channel &chan = Channel::getChannelByName(*_chan_lst, data.first);

		if (!chan.getPassword().empty() && chan.getPassword() != data.second) {
			throw (std::invalid_argument("Error : wrong password."));
		}
		if (chan.isModeSet('i') && !Channel::isInvitedUserInChannel(chan, *_user))
			throw (std::invalid_argument("Error : not invited user."));
		if (chan.isModeSet('l') && chan.getUserLimit() == chan.getUsers().size())
			throw (std::invalid_argument("Error : channel already full."));
		if (chan.isModeSet('i'))
			chan.removeInvitedUser(*_user);
		chan.addUser(*_user);
		_user->addChannel(chan);
		for (UserLst::iterator it = chan.getUsers().begin(); it != chan.getUsers().end(); it++) {
			Messages::sendMsg(it->getFd(), chan.getName(), *_user, JOIN);
		}
		if (!chan.getTopic().empty())
			Messages::sendServMsg(_user->getFd(), _user->getNick() + " " + chan.getName() + " " + chan.getTopic(), "332");
	}
	catch (std::exception &e) {
		if (std::string(e.what()) == "Channel not found.")
			throw Error(_user->getFd(), *_user, ERR_NOSUCHCHANNEL, NOSUCHCHANNEL_MSG(data.first));
		else if (std::string(e.what()) == "Error : wrong password.")
			throw Error(_user->getFd(), *_user, ERR_BADCHANNELKEY, BADCHANNELKEY_MSG(data.first));
		else if (std::string(e.what()) == "Error : not invited user.")
			throw Error(_user->getFd(), *_user, ERR_INVITEONLYCHAN, ERR_INVITEONLYCHAN_MSG(data.first));
		else if ((std::string(e.what()) == "Error : channel already full."))
			throw Error(_user->getFd(), *_user, ERR_CHANNELISFULL, ERR_CHANNELISFULL_MSG(data.first));
		else if ((std::string(e.what()) == "The user is already in the channel."))
			throw Error(_user->getFd(), *_user, ERR_ALREADYREGISTRED, ALREADYREGISTRED_MSG);
	}
}

void	JoinCmd::createJoinChannel(const std::pair<std::string, std::string> &data) {
	Channel chan(data.first, data.second);
	chan.addUser(*_user);
	chan.addOperator(*_user);
	_user->addChannel(chan);
	_chan_lst->push_back(chan);
	Messages::sendMsg(_user->getFd(), data.first, *_user, JOIN);
}

void	JoinCmd::joinOneChannel(const std::pair<std::string, std::string> &data)
{
	try {
	    this->joinChannel(data);
	}
	catch (Error &err) {
		if (err.getType() != ERR_NOSUCHCHANNEL) {
			throw err;
		}
		try {
	        this->createJoinChannel(data);
		}
		catch (std::exception &e) {
			throw Error(_user->getFd(), *_user, ERR_NOSUCHCHANNEL, NOSUCHCHANNEL_MSG(data.first));
		}
	}
}

void JoinCmd::execute(int fd) {
	if (_data.empty())
		throw Error(fd, *_user, ERR_NEEDMOREPARAMS, NEEDMOREPARAMS_MSG("JOIN"));
	if (_data == "0") {
		std::string	part_param;
		for (ChannelLst::iterator it = _user->getChannels().begin(); it != _user->getChannels().end(); it++) {
			part_param += it->getName();
			if (it + 1 != _chan_lst->end())
				part_param += ",";
		}
		part_param += " :Leaving";
		PartCmd part(*_user, *_user_lst, *_chan_lst, part_param);
		part.execute(fd);
		return ;
	}
	_channel_part = _data.substr(0, _data.find(' '));
	if (_data.find(' ') == std::string::npos)
		_mdp_part = "";
	else
		_mdp_part = _data.substr(_data.find(' ') + 1);
	std::map<std::string, std::string>	channels = this->splitData();
	for (std::map<std::string, std::string>::iterator it = channels.begin(); it != channels.end(); it++) {
		try {
			this->joinOneChannel(*it);
			NamesCmd names(*_user, *_user_lst, *_chan_lst, it->first);

			names.execute(fd);
		}
		catch (Error &e) {
			e.sendError();
		}
	}
}

std::map<std::string, std::string>	JoinCmd::splitData() {
	std::map<std::string, std::string>	channels_mdp;
	std::vector<std::string>			channels;
	std::vector<std::string>			mdps;
	std::stringstream					storage_chan(_channel_part);
	std::stringstream					storage_mdp(_mdp_part);
	std::string							segment;

	while (std::getline(storage_chan, segment, ','))
		channels.push_back(segment);
	while (std::getline(storage_mdp, segment, ','))
		mdps.push_back(segment);

	for (std::vector<std::string>::iterator it_chan = channels.begin(), it_mdp = mdps.begin() ; it_chan != channels.end(); it_chan++) {
		if (it_chan->at(0) != '#') {
			Error(_user->getFd(), *_user, ERR_NOSUCHCHANNEL, NOSUCHCHANNEL_MSG(*it_chan));
			if (it_mdp != mdps.end())
				it_mdp++;
			continue ;
		}
		if (it_mdp != mdps.end()) {
			channels_mdp.insert(std::make_pair(*it_chan, *it_mdp));
			it_mdp++;
		}
		else
			channels_mdp.insert(std::make_pair(*it_chan, ""));
	}
	return (channels_mdp);
}
