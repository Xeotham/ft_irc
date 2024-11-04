#ifndef LISTCMD_HPP
# define LISTCMD_HPP

# include "ACommand.hpp"

class ListCmd : public ACommand{
	public:
		ListCmd(Client &user, UserLst &user_lst, ChannelLst &chan_lst, const std::string &data);
		ListCmd(const ListCmd &other);
		~ListCmd();
	public:
		ListCmd &operator=(const ListCmd &other);
	public:
		void execute(int fd);
	private:
		ListCmd();
	private:
		void					listChannels(std::vector<Channel> &lst);
		std::vector<Channel>	splitData();
};

#endif //LISTCMD_HPP
