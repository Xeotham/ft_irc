#include <NickCmd.hpp>


NickCmd::NickCmd() {}

NickCmd::NickCmd(const NickCmd &other) : ACommand(other) {}

NickCmd::NickCmd(Client &user, UserLst &user_lst, ChannelLst &chan_lst, const std::string &data) : ACommand(user, user_lst, chan_lst, data) {}

NickCmd::~NickCmd() {}

NickCmd &NickCmd::operator=(const NickCmd &other) {
	if (this == &other)
		return *this;
	return *this;
}

void	NickCmd::sendToEveryone() {
	std::set<int>	receiver;

	receiver.insert(_user->getFd());
	for (ChannelLst::iterator it_chan = _user->getChannels().begin(); it_chan != _user->getChannels().end(); ++it_chan) {
		for (UserLst::iterator it_user = it_chan->getUsers().begin(); it_user != it_chan->getUsers().end(); ++it_user)
			receiver.insert(it_user->getFd());
	}
	for (std::set<int>::iterator it = receiver.begin(); it != receiver.end(); ++it) {
		std::cout << "Sending NICK to <" << *it << ">" << std::endl;
		Messages::sendMsg(*it, _data, *_user, NICK);
	}
}

void NickCmd::execute(int fd) {
	if (_data.empty())
		throw Error(fd, *_user, ERR_NONICKNAMEGIVEN, NONICKNAMEGIVEN_MSG);
	if (_data.find_first_of(' ') != std::string::npos)
		_data.erase(_data.find_first_of(' '));
	try {
		Client	&tmp = Client::getClientByNick(*_user_lst, _data);
		static_cast<void> (tmp);
		if (_user->getUser().empty()) {
			close(fd);
			// need to clear client from user_lst
		}
		throw Error(fd, *_user, ERR_NICKNAMEINUSE, NICKNAMEINUSE_MSG(_data));
	}
	catch (Error &e) {
		throw e;
	}
	catch (...) {
		this->sendToEveryone();
		_user->setNick(_data);
		std::cout << "Client <" << fd << "> set nickname to : " << _user->getNick() << std::endl;
	}
}
