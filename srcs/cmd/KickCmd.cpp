#include <KickCmd.hpp>

KickCmd::KickCmd(){}

KickCmd::KickCmd(Client &user, UserLst &user_lst, ChannelLst &chan_lst, const std::string &data)
	: ACommand(user, user_lst, chan_lst, data){}

KickCmd::~KickCmd(){}

void KickCmd::execute(int fd)
{
	std::string	channel;
	std::string	kicked_user;
	std::string	comment;

	std::istringstream iss(_data);
    std::getline(iss, channel, ' ');
    std::getline(iss, kicked_user, ' ');
    std::getline(iss, comment);

	if (channel.empty() || kicked_user.empty())
	{
		Messages::sendServMsg(fd, "KICK " + _data + " :Not enough parameters", "461 " + _user->getNick());
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
		Messages::sendServMsg(fd, channel + " :No such nick/channel", "401 " + _user->getNick());
		return ;
	}
	
	Channel& target_channel = Channel::getChannelByName(*_chan_lst, channel);
	if (!Client::isClientInList(target_channel.getUsers(), _user->getNick()))
	{
		Messages::sendServMsg(fd, channel + " :You're not on that channel", "442 " + _user->getNick());
		return ;
	}

	UserLst& operator_list = target_channel.getOperators();	
	for (UserLst::iterator it = operator_list.begin(); it != operator_list.end() + 1; ++it)
	{
		if (it == operator_list.end())
		{
			Messages::sendServMsg(fd, channel + " :You're not channel operator", "482 " + _user->getNick());
			return ;
		}
		if (it->getNick() == _user->getNick())
			break ;
	}

	for (UserLst::iterator ti = _user_lst->begin(); ti != _user_lst->end(); ++ti)
	{
		if (ti->getNick() == kicked_user)
		{
			Client& kicked_client = *ti;
			std::string message = target_channel.getName() + " " + kicked_client.getNick();
			if (comment.empty())
				Messages::sendGlobalMsg(target_channel.getUsers(), message + " :" + kicked_client.getNick(), *_user, "KICK");
			else
				Messages::sendGlobalMsg(target_channel.getUsers(), message + " " + comment, *_user, "KICK");
			target_channel.removeUser(kicked_client);
			kicked_client.removeChannel(target_channel);
			break ;
		}
	}
}
