#ifndef PINGCMD_HPP
# define PINGCMD_HPP

# include "ACommand.hpp"

class PingCmd : public ACommand {
	public:
		PingCmd(UserLst &user_lst, ChannelLst &chan_lst, const std::string &data);
		PingCmd(const PingCmd &other);
		~PingCmd();
	public:
		PingCmd &operator=(const PingCmd &other);
	public:
		void	execute(int fd);
	private:
		PingCmd();
};

#endif
