#ifndef ACOMMAND_HPP
# define ACOMMAND_HPP

# include "Params.hpp"

class ACommand {
	public:
		ACommand();
		ACommand(const ACommand &other);
		virtual ~ACommand();
	public:
		ACommand &operator=(const ACommand &other);
	public:
        virtual void execute(int fd, const std::string &data, ChannelLst &chan_lst, UserLst &user_lst) = 0;
};



#endif
