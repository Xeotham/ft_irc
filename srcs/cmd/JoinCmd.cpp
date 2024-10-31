#include <JoinCmd.hpp>
#include <Channel.hpp>
#include <NamesCmd.hpp>
#include <PartCmd.hpp>

JoinCmd::JoinCmd() : ACommand(){}

JoinCmd::JoinCmd(const JoinCmd &other) : ACommand(other) {
	*this = other;
}

JoinCmd::JoinCmd(UserLst &user_lst, ChannelLst &chan_lst, const std::string &data) : ACommand(user_lst, chan_lst, data) {}

JoinCmd::~JoinCmd() {}

JoinCmd &JoinCmd::operator=(const JoinCmd &other) {
	if (this == &other)
		return (*this);
	return (*this);
}

void	JoinCmd::joinChannel(Client &user, const std::pair<std::string, std::string> &data) {
	try {
		Channel &chan = Channel::getChannelByName(*_chan_lst, data.first);

		std::cout << "When joining Channel " << chan.getName() << " & " << chan.getPassword() << std::endl;
		if (!chan.getPassword().empty() && chan.getPassword() != data.second)
			throw (std::invalid_argument("Error : wrong password."));
		if (chan.isModeSet('i') && !Channel::isInvitedUserInChannel(chan, user))
			throw (std::invalid_argument("Error : not invited user."));		
		if (chan.isModeSet('l') && chan.getUserLimit() == chan.getUsers().size())
			throw (std::invalid_argument("Error : channel already full."));
		chan.removeInvitedUser(user);
		chan.addUser(user);
		user.addChannel(chan);
		for (UserLst::iterator it = chan.getUsers().begin(); it != chan.getUsers().end(); it++) {
			if (it->getNick() == user.getNick())
				continue ;
			Messages::sendMsg(it->getFd(), chan.getName(), user, JOIN);
		}
	}
	catch (std::exception &e) {
		if (std::string(e.what()) == "Channel not found.")
			throw Error(user.getFd(), user, ERR_NOSUCHCHANNEL, NOSUCHCHANNEL_MSG(data.first));
		else if (std::string(e.what()) == "Error : wrong password.")
			throw Error(user.getFd(), user, ERR_BADCHANNELKEY, BADCHANNELKEY_MSG(data.first));
		else if (std::string(e.what()) == "Error : not invited user.")
			throw Error(user.getFd(), user, ERR_INVITEONLYCHAN, ERR_INVITEONLYCHAN_MSG(data.first));
		else if ((std::string(e.what()) == "Error : channel already full."))
			throw Error(user.getFd(), user, ERR_CHANNELISFULL, ERR_CHANNELISFULL_MSG(data.first));
		else
			std::cout << "UNHANDLE ERROR" << std::endl; // to delete // maxime
	}

}

void	JoinCmd::createJoinChannel(Client user, const std::pair<std::string, std::string> &data) {
	std::cout << "When creating Channel " << data.first << " & " << data.second << std::endl;
	Channel chan(data.first, data.second);
	std::cout << "When channel created " << chan.getName() << " & " << chan.getPassword() << std::endl;
	chan.addUser(user);
	chan.addOperator(user);
	user.addChannel(chan);
	_chan_lst->push_back(chan);
}

void	JoinCmd::joinOneChannel(Client &user, const std::pair<std::string, std::string> &data)
{
	try {
	    this->joinChannel(user, data);
	}
	catch (Error &err) {
		if (err.getType() == ERR_BADCHANNELKEY || err.getType() == ERR_INVITEONLYCHAN || err.getType() == ERR_CHANNELISFULL)
			throw err;
		try {
	        this->createJoinChannel(user, data);
		}
		catch (std::exception &e) {
			throw (std::invalid_argument(e.what()));
		}
	}
}

void JoinCmd::execute(int fd) {
	Client &user = Client::getClientByFd(*_user_lst, fd);

	if (_data.empty())
		throw Error(fd, user, ERR_NEEDMOREPARAMS, NEEDMOREPARAMS_MSG("JOIN"));
	if (_data == "0") {
		std::string	part_param;
		for (ChannelLst::iterator it = _chan_lst->begin(); it != _chan_lst->end(); it++) {
			part_param += it->getName();
			if (it + 1 != _chan_lst->end())
				part_param += ",";
		}
		part_param += " :Leaving\r\n";
		PartCmd part(*_user_lst, *_chan_lst, part_param);
		part.execute(fd);
		return ;
	}
	std::map<std::string, std::string>	channels = this->splitData();
	std::cout << user.getNick() << " try to join" << std::endl;
	for (std::map<std::string, std::string>::iterator it = channels.begin(); it != channels.end(); it++) {
		try {
			this->joinOneChannel(user, *it);
			Messages::sendMsg(fd, it->first, user, JOIN);
			NamesCmd names(*_user_lst, *_chan_lst, it->first);
			names.execute(fd);
		}
		catch (std::exception &e) {
			Messages::sendServMsg(fd, e.what(), JOIN);
		}
	}
}

std::map<std::string, std::string>	JoinCmd::splitData() {
	std::map<std::string, std::string>	channels_mdp;
	std::vector<std::string>			channels;
	std::stringstream					storage(_data);
	std::string							segment;
	std::vector<std::string>::iterator	iter;

	while (std::getline(storage, segment, ','))
		channels.push_back(segment);
	for (iter = channels.begin(); iter != channels.end(); iter++) {
		if (iter->at(0) == '#')
			continue ;
		break ;
	}
	if (iter == channels.begin()) {
		channels_mdp.insert(std::make_pair("invalid", ""));
		return (channels_mdp);
	}
	for (std::vector<std::string>::iterator it = channels.begin(); it != channels.end(); it++) {
		if (it->at(0) != '#')
			break ;
		if (iter == channels.end()) {
			channels_mdp.insert(std::make_pair(*it, ""));
		}
		else {
			channels_mdp.insert(std::make_pair(*it, *iter));
			iter++;
		}
	}
	return (channels_mdp);
}
