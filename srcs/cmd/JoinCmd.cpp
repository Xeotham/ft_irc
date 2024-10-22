#include <JoinCmd.hpp>
#include <Channel.hpp>

JoinCmd::JoinCmd() : ACommand() {}

JoinCmd::JoinCmd(const JoinCmd &other) : ACommand(other) {
	*this = other;
}

JoinCmd::~JoinCmd() {}

JoinCmd &JoinCmd::operator=(const JoinCmd &other) {
	if (this == &other)
		return (*this);
	return (*this);
}

void	JoinCmd::joinChannel(Client &user, const std::pair<std::string, std::string> &data, ChannelLst &chan_lst) {
	Channel &chan = Channel::getChannelByName(chan_lst, data.first);
	std::cout << "When joining Channel " << chan.getName() << " & " << chan.getPassword() << std::endl;
	if (!chan.getPassword().empty() && chan.getPassword() != data.second)
		throw (std::invalid_argument("Error : wrong password."));
	chan.addUser(user);
	user.addChannel(chan.getName());
}

void	JoinCmd::createJoinChannel(Client &user, const std::pair<std::string, std::string> &data, ChannelLst &chan_lst) {
	std::cout << "When creating Channel " << data.first << " & " << data.second << std::endl;
	Channel chan(data.first, data.second);
	std::cout << "When channel created " << chan.getName() << " & " << chan.getPassword() << std::endl;
	chan.addUser(user);
	user.addChannel(chan.getName());
	chan_lst.push_back(chan);
}


void	JoinCmd::joinOneChannel(Client &user, const std::pair<std::string, std::string> &data, ChannelLst &chan_lst)
{
	try {
	    this->joinChannel(user, data, chan_lst);
	}
	catch (std::exception &ex) {
		if (std::string(ex.what()) == "Error : wrong password.")
			throw (std::invalid_argument("Error : wrong password to join " + data.first + "."));
		try {
	        this->createJoinChannel(user, data, chan_lst);
		}
		catch (std::exception &e) {
			throw (std::invalid_argument(e.what()));
		}
	}
}


void JoinCmd::execute(int fd, const std::string &data, ChannelLst &chan_lst, UserLst &user_lst) {
	Client &user = Client::getClientByFd(user_lst, fd);
	if (data == "0") {
		while (!user.getChannels().empty()) {
			Messages::sendMsg(fd, *user.getChannels().begin() + " leaving", user, PART);
			user.removeChannel(*user.getChannels().begin());
		}
		return ;
	}
	std::map<std::string, std::string>	channels = this->splitData(data);
	std::cout << user.getNick() << " try to join" << std::endl;
	for (std::map<std::string, std::string>::iterator it = channels.begin(); it != channels.end(); it++) {
		try {
			this->joinOneChannel(user, *it, chan_lst);
			Messages::sendMsg(fd, it->first, user, JOIN);
		}
		catch (std::exception &e) {
			std::string msg(std::string(e.what()) + "\r\n");
			send(fd, msg.c_str(), msg.size(), 0);
		}
	}
}

std::map<std::string, std::string>	JoinCmd::splitData(const std::string &data) {
	std::map<std::string, std::string>	channels_mdp;
	std::vector<std::string>			channels;
	std::stringstream					storage(data);
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
