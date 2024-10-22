#ifndef ACOMMAND_HPP
# define ACOMMAND_HPP

# include "Params.hpp"

class ACommand {
	public:
		ACommand(const ACommand &other);
		// ACommand(UserLst &user_lst, ChannelLst &chan_lst, std::string data);
		virtual ~ACommand();
	public:
		ACommand &operator=(const ACommand &other);
	public:
		virtual void execute(int fd, const std::string &data, ChannelLst &chan_lst, UserLst &user_lst) = 0;
	// protected:
	// 	UserLst		&_user_lst;
	// 	ChannelLst	&_chan_lst;
	// 	std::string	_data;
	protected:
		ACommand();
};



#endif
