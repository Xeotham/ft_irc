#ifndef PARTCMD_HPP
# define PARTCMD_HPP

# include "ACommand.hpp"

class PartCmd : public ACommand {
	public:
		PartCmd();
		PartCmd(const PartCmd &other);
		virtual ~PartCmd();
	public:
		PartCmd &operator=(const PartCmd &other);
	public:
		void execute(int fd, const std::string &data, ChannelLst &chan_lst, UserLst &user_lst);
	private:
		std::vector<std::string> splitData(const std::string &data);
//		void	partChannel(Client &user, const std::string &channel, ChannelLst &chan_lst);
//		void	partAllChannels(Client &user, ChannelLst &chan_lst);
//		void	partOneChannel(Client &user, const std::string &channel, ChannelLst &chan_lst);

};

#endif
