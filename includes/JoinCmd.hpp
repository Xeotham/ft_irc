#ifndef JOINCMD_HPP
# define JOINCMD_HPP

# include "ACommand.hpp"
# include "Client.hpp"

class JoinCmd : public ACommand
{
	public:
		JoinCmd(UserLst &user_lst, ChannelLst &chan_lst, const std::string &data);
		JoinCmd(const JoinCmd &other);
		virtual ~JoinCmd();
	public:
        JoinCmd &operator=(const JoinCmd &other);
    public:
		void execute(int fd);
	private:
		JoinCmd();
	private:
		void	createJoinChannel(Client user, const std::pair<std::string, std::string> &data);
		void	joinChannel(Client &user, const std::pair<std::string, std::string> &data);
		void	joinOneChannel(Client &user, const std::pair<std::string, std::string> &data);
		std::map<std::string, std::string> splitData();
};

#endif
