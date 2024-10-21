#ifndef JOINCMD_HPP
# define JOINCMD_HPP

# include "ACommand.hpp"
# include "Client.hpp"

class JoinCmd : public ACommand
{
	public:
		JoinCmd();
		JoinCmd(const JoinCmd &other);
		virtual ~JoinCmd();
	public:
        JoinCmd &operator=(const JoinCmd &other);
    public:
		void execute(int fd, const std::string &data, ChannelLst &chan_lst, UserLst &user_lst);
	private:
		void	createJoinChannel(Client &user, const std::pair<std::string, std::string> &data, ChannelLst &chan_lst);
		void	joinChannel(Client &user, const std::pair<std::string, std::string> &data, ChannelLst &chan_lst);
		void	joinOneChannel(Client &user, const std::pair<std::string, std::string> &data, ChannelLst &chan_lst);
		std::map<std::string, std::string> splitData(const std::string &data);
};

#endif
