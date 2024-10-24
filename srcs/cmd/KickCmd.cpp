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
	Client 		&user = Client::getClientByFd(*_user_lst, fd);
	std::string	channel;
	std::string	kicked_user;
	std::string	comment;

	std::istringstream iss(_data.c_str());
	if (!(iss >> channel >> kicked_user >> comment))
	{
		std::cerr << "Error: Invalid data format" << std::endl;
		return;
	}

	if (channel.empty() || kicked_user.empty())
	{
		Messages::sendMsg(fd, "KICK " + _data + " :Not enough parameters", user, "461");
		return;
	}
	

	try
	{
		Channel::getChannelByName(*_chan_lst, channel);
	}
	catch(std::invalid_argument& e)
	{
		
	}
	
	
	(void) user;
	// std::string	

};
