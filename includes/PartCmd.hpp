#ifndef PARTCMD_HPP
# define PARTCMD_HPP

# include "ACommand.hpp"

class PartCmd : public ACommand {
	public:
		PartCmd(Client &user, UserLst &user_lst, ChannelLst &chan_lst, const std::string &data);
		PartCmd(const PartCmd &other);
		virtual ~PartCmd();
	public:
		PartCmd &operator=(const PartCmd &other);
	public:
		void	execute(int fd);
	private:
		PartCmd();
	private:
		void						exitChannel(Channel &chan, const std::string &msg);
		std::vector<std::string>	splitData();
};

#endif
