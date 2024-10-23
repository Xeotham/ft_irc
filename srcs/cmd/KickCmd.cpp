#include "KickCmd.hpp"

#include "stdio.h"

KickCmd::KickCmd(){};

KickCmd::KickCmd(UserLst &user_lst, ChannelLst &chan_lst, const std::string &data)
	: ACommand(user_lst, chan_lst, data)
{
	std::cout << "Datas: " << data << std::endl;
};

KickCmd::~KickCmd(){};

void KickCmd::execute(int fd)
{
	(void)fd;
};
