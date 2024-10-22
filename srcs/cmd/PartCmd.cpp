#include <PartCmd.hpp>
#include <Channel.hpp>

PartCmd::PartCmd() : ACommand(){}

PartCmd::PartCmd(const PartCmd &other) : ACommand(other) {
	*this = other;
}

PartCmd::~PartCmd() {}

PartCmd &PartCmd::operator=(const PartCmd &other) {
	if (this == &other)
		return (*this);
	return (*this);
}

void PartCmd::execute(int fd, const std::string &data, ChannelLst &chan_lst, UserLst &user_lst) {
	std::vector<std::string>	channels = splitData(data.substr(0, data.find(':') - 1));
	std::string					msg = data.substr(data.find(':') + 1);
	Client						&user = Client::getClientByFd(user_lst, fd);

	for (std::vector<std::string>::iterator iter = channels.begin(); iter != channels.end(); iter++) {
		if (iter->find("invalid ") != std::string::npos) {
			std::string err_msg = "Channel " + iter->substr(8) + " is not a valid channel name\r\n";
        	send(fd, err_msg.c_str(), err_msg.size(), 0);
        	continue ;
        }
        try {
        	Channel &chan = Channel::getChannelByName(chan_lst, *iter);
			if (Channel::isUserInChannel(chan, user)) {
				Messages::sendMsg(fd, *iter + " " + msg, user, PART);
				chan.removeUser(user);
                user.removeChannel(chan.getName());
			}
		}
		catch (std::exception &e) {
			std::string err_msg = user.getNick() + " is not in the " + *iter + " channel.\r\n";
			send(fd, err_msg.c_str(), err_msg.size(), 0);
        }

	}
}

std::vector<std::string>	PartCmd::splitData(const std::string &data) {
	std::vector<std::string>			channels;
	std::stringstream					storage(data);
	std::string							segment;

	while (std::getline(storage, segment, ','))
		channels.push_back(segment);
	for (std::vector<std::string>::iterator iter = channels.begin(); iter != channels.end(); iter++) {
		if (iter->at(0) == '#')
			continue ;
		else
			*iter = "invalid " + *iter;
	}
	return (channels);
}
