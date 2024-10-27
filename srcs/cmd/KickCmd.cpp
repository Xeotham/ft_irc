#include "KickCmd.hpp"

KickCmd::KickCmd(){}

KickCmd::KickCmd(UserLst &user_lst, ChannelLst &chan_lst, const std::string &data)
	: ACommand(user_lst, chan_lst, data){}

KickCmd::~KickCmd(){}

void KickCmd::execute(int fd)
{
	Client&		user = Client::getClientByFd(*_user_lst, fd);
	std::string	channel;
	std::string	kicked_user;
	std::string	comment;

	std::istringstream iss(_data);
    std::getline(iss, channel, ' ');
    std::getline(iss, kicked_user, ' ');
    std::getline(iss, comment);

	if (channel.empty() || kicked_user.empty())
	{
		Messages::sendMsg(fd, "KICK " + _data + " :Not enough parameters", user, "461");
		return ;
	}

	try
	{
		Channel& target_channel = Channel::getChannelByName(*_chan_lst, channel);
		if (!Client::isClientInList(target_channel.getUsers(), kicked_user))
			throw (std::invalid_argument("Customer not found."));
	}
	catch (std::invalid_argument& e)
	{
		Messages::sendMsg(fd, channel + " :No such nick/channel", user, "401");
		return ;
	}
	
	Channel& target_channel = Channel::getChannelByName(*_chan_lst, channel);
	if (!Client::isClientInList(target_channel.getUsers(), user.getNick()))
	{
		Messages::sendMsg(fd, channel + " :You're not on that channel", user, "442");
		return ;
	}

	UserLst& operator_list = target_channel.getOperators();	
	for (UserLst::iterator it = operator_list.begin(); it != operator_list.end() + 1; ++it)
	{
		if (it == operator_list.end())
		{
			Messages::sendMsg(fd, channel + " :You're not channel operator", user, "482");
			return ;
		}
		if (it->getNick() == user.getNick())
			break ;
	}

	for (UserLst::iterator ti = _user_lst->begin(); ti != _user_lst->end(); ++ti)
	{
		if (ti->getNick() == kicked_user)
		{
			Client& kicked_client = *ti;
			std::string message = target_channel.getName() + " " + kicked_client.getNick() + " :";
			if (comment.empty())
				Messages::sendGlobalMsg(target_channel.getUsers(), message + kicked_client.getNick(), user, "KICK");
			else
				Messages::sendGlobalMsg(target_channel.getUsers(), message + comment, user, "KICK");
			target_channel.removeUser(kicked_client);
			kicked_client.removeChannel(target_channel);
			break ;
		}
	}
}
