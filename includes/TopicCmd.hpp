#ifndef Topic_HPP
# define Topic_HPP

#include "ACommand.hpp"
#include "Client.hpp"

class TopicCmd : public ACommand 
{
	private:
		TopicCmd();
	public:
		TopicCmd(UserLst &user_lst, ChannelLst &chan_lst, const std::string &data);
		virtual ~TopicCmd();
	public:
		void execute(int fd);
};

#endif