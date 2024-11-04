#ifndef JOINCMD_HPP
# define JOINCMD_HPP

# include "ACommand.hpp"
# include "Client.hpp"

class JoinCmd : public ACommand
{
	public:
		JoinCmd(Client &user, UserLst &user_lst, ChannelLst &chan_lst, const std::string &data);
		JoinCmd(const JoinCmd &other);
		~JoinCmd();
	public:
        JoinCmd &operator=(const JoinCmd &other);
    public:
		void execute(int fd);
	private:
		JoinCmd();
	private:
		void	createJoinChannel(const std::pair<std::string, std::string> &data);
		void	joinChannel(const std::pair<std::string, std::string> &data);
		void	joinOneChannel(const std::pair<std::string, std::string> &data);
		std::map<std::string, std::string> splitData();
	private:
		std::string	_channel_part;
		std::string	_mdp_part;
};

#endif
