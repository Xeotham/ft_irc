#ifndef WHOCMD_HPP
# define WHOCMD_HPP

# include "ACommand.hpp"

class WhoCmd : public ACommand {
	public:
		WhoCmd(const WhoCmd &other);
		WhoCmd(Client &user, UserLst &user_lst, ChannelLst &chan_lst, const std::string &data);
		~WhoCmd();
	public:
		WhoCmd &operator=(const WhoCmd &other);
	public:
		void execute(int fd);
	private:
		WhoCmd();
	private:
		void	sendData(int fd, Channel &chan);
		void	sendData(int fd, Client &target);
};

#endif