#ifndef NAMESCMD_HPP
# define NAMESCMD_HPP

# include "ACommand.hpp"

class NamesCmd : public ACommand {
	public:
		NamesCmd(Client &user, UserLst &user_lst, ChannelLst &chan_lst, const std::string &data);
		NamesCmd(const NamesCmd &other);
		~NamesCmd();
	public:
		NamesCmd &operator=(const NamesCmd &other);
	public:
		void	execute(int fd);
	private:
		NamesCmd();
	private:
		void						sendAllNames(ChannelLst &chan_lst);
		void						sendAllNames(std::vector<std::string> &chan_lst);
		void						sendNames(Channel &chan);
		std::vector<std::string>	splitData();
};

#endif
