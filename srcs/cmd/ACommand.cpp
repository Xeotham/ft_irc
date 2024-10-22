#include <ACommand.hpp>

ACommand::ACommand() {}

ACommand::ACommand(const ACommand &other) {
	*this = other;
}

// ACommand::ACommand(UserLst &user_lst, ChannelLst &chan_lst, std::string data) : user_lst(user_lst), chan_lst(chan_lst)
// {}

ACommand::~ACommand() {}

ACommand &ACommand::operator=(const ACommand &other) {
	if (this == &other)
		return (*this);
	return (*this);
}
