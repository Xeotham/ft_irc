#include <PingCmd.hpp>

PingCmd::PingCmd(Client &user, UserLst &user_lst, ChannelLst &chan_lst, const std::string &data) : ACommand(user, user_lst, chan_lst, data){}

PingCmd::PingCmd(const PingCmd &other) : ACommand(other){}

PingCmd::~PingCmd(){}

PingCmd &PingCmd::operator=(const PingCmd &other)
{
	if (this == &other)
		return (*this);
	return (*this);
}

void PingCmd::execute(int fd) {
	if (_data.empty())
		throw Error(fd, *_user, ERR_NOORIGIN, NOORIGIN_MSG);
	Messages::sendServMsg(fd, SERVER_NAME + " :" + _data, "PONG");
}
