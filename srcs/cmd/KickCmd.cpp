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

	
	std::cout << "This is the data in kick: " << _data << std::endl;
	// try
	// {

	// }
	// catch(std::invalid_argument& e)
	// {

	// }
	
	
	(void) user;
	// std::string	

};
