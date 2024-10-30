#ifndef NAMESCMD_HPP
# define NAMESCMD_HPP

# include "ACommand.hpp"

class NamesCmd : public ACommand {
	public:
		NamesCmd(UserLst &user_lst, ChannelLst &chan_lst, const std::string &data);
		NamesCmd(const NamesCmd &other);
		~NamesCmd();
	public:
		NamesCmd &operator=(const NamesCmd &other);
	public:
		void	execute(int fd);
	private:
		NamesCmd();
	private:
		void						sendAllNames(Client &user, ChannelLst &chan_lst);
		void						sendAllNames(Client &user, std::vector<std::string> &chan_lst);
		void						sendNames(Client &user, Channel &chan);
		std::vector<std::string>	splitData();
};

#endif
