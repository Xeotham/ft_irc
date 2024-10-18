#ifndef JOINCMD_H
#define JOINCMD_H

#include <ACommand.hpp>
#include <Client.hpp>

class JoinCmd : public ACommand
{
	public:
		JoinCmd();
		JoinCmd(const JoinCmd &other);
		virtual ~JoinCmd();
	public:
        JoinCmd &operator=(const JoinCmd &other);
    public:
		void execute(int fd, const std::string &data, ChannelLst &chan_lst);
	private:
		void joinOneChannel(Client &user, const std::pair<std::string, std::string> &data);
		void splitData(const std::string &data);
};



#endif //JOINCMD_H
