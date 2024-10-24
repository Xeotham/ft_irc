#include "KickCmd.hpp"
#include "stdio.h"

KickCmd::KickCmd(){};

KickCmd::KickCmd(UserLst &user_lst, ChannelLst &chan_lst, const std::string &data)
	: ACommand(user_lst, chan_lst, data){};

KickCmd::~KickCmd(){};

/*
	My datas:
		- Data: ICK mine ss
		- Channel list: of the server
		- Client list
*/
void KickCmd::execute(int fd)
{
	Client &user = Client::getClientByFd(*_user_lst, fd);

	std::cout << "START EXE" << std::endl;
	std::cout << "The user is: " << user.getNick() << std::endl;

	(void)fd;


	std::istringstream iss(_data);
	std::string command, channelName, userName, reason;
	iss >> command >> channelName >> userName;
	std::getline(iss, reason);
	reason = reason.empty() ? "No reason given" : reason.substr(1);

	// Print values of user_lst, chan_lst, and data
	std::cout << "Data: " << _data << std::endl;
	std::cout << "User List:" << std::endl;
	for (UserLst::iterator it = _user_lst->begin(); it != _user_lst->end(); ++it) {
		std::cout << " - " << it->getNick() << std::endl;
	}
	std::cout << "Channel List:" << std::endl;
	for (ChannelLst::iterator it = _chan_lst->begin(); it != _chan_lst->end(); ++it) {
		std::cout << " - " << it->getName() << std::endl;
	}

	ChannelLst::iterator chanIt = _chan_lst->begin();
	while (chanIt != _chan_lst->end()) {
		if (chanIt->getName() == channelName) {
			break;
		}
		++chanIt;
	}

	if (chanIt == _chan_lst->end()) {
		// Channel not found
		return;
	}

	UserLst::iterator userIt = _user_lst->begin();
	while (userIt != _user_lst->end()) {
		if (userIt->getNick() == userName) {
			break;
		}
		++userIt;
	}

	if (userIt == _user_lst->end()) {
		// User not found
		return;
	}

	chanIt->removeUser(*userIt);

};
