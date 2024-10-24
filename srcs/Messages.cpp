#include <Messages.hpp>

Messages::Messages()
{
}

Messages::Messages(const Messages &ref)
{
	*this = ref;
}

Messages	&Messages::operator=(const Messages &ref)
{
	if (this == &ref)
		return *this;
	return *this;
}

Messages::~Messages()
{
}

void	Messages::sendMsg(int fd, const std::string &msg, const Client &sender, const std::string &type)
{
	std::string	message = ":" + sender.getNick() + "!" + sender.getUser() + "@localhost " + type + " " + msg + "\r\n";
	std::cout << "Messages: " << message << std::endl;
	send(fd, message.c_str(), message.size(), 0);
}
