#include <Error.hpp>
#include <string>

class Client;
Error::Error(int fd, const Client &sender, const std::string &type, const std::string &msg) : _sender(sender), _fd(fd), _type(type), _msg(msg) {}

Error::Error(const Error &other) :_sender(other._sender), _fd(other._fd), _type(other._type), _msg(other._msg) {}

Error::~Error() {}

Error &Error::operator=(const Error &other) {
	if (this == &other)
		return *this;
	return *this;
}

const std::string &Error::getType() const {
	return (this->_type);
}


void Error::sendError() const {
	Messages::sendServMsg(this->_fd, this->_sender.getNick() + " " + this->_msg, this->_type);
	std::cerr << ":" << SERVER_NAME << " " << this->_type << " " << this->_msg << std::endl;
}
