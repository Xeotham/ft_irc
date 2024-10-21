#include <ACommand.hpp>

ACommand::ACommand() {}

ACommand::ACommand(const ACommand &other) {
	*this = other;
}

ACommand::~ACommand() {}

ACommand &ACommand::operator=(const ACommand &other) {
	if (this == &other)
		return *this;
	return *this;
}
