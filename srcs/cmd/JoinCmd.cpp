#include <JoinCmd.hpp>
#include <Channel.hpp>

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
	Channel &chan = Channel::getChannelByName(*_chan_lst, data.first);
	std::cout << "When joining Channel " << chan.getName() << " & " << chan.getPassword() << std::endl;
	if (!chan.getPassword().empty() && chan.getPassword() != data.second)
		throw (std::invalid_argument("Error : wrong password."));
	chan.addUser(user);
	user.addChannel(chan);
}

void	JoinCmd::createJoinChannel(Client &user, const std::pair<std::string, std::string> &data) {
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
	catch (std::exception &ex) {
		if (std::string(ex.what()) == "Error : wrong password.")
			throw (std::invalid_argument("Error : wrong password to join " + data.first + "."));
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
	if (_data == "0") {
		while (!user.getChannels().empty()) {
			Messages::sendMsg(fd, user.getChannels().begin()->getName() + " leaving", user, PART);
			user.removeChannel(*user.getChannels().begin());
		}
		return ;
	}
	std::map<std::string, std::string>	channels = this->splitData();
	std::cout << user.getNick() << " try to join" << std::endl;
	for (std::map<std::string, std::string>::iterator it = channels.begin(); it != channels.end(); it++) {
		try {
			this->joinOneChannel(user, *it);
			Messages::sendMsg(fd, it->first, user, JOIN);
		}
		catch (std::exception &e) {
			std::string msg(std::string(e.what()) + "\r\n");
			send(fd, msg.c_str(), msg.size(), 0);
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
