#include "InviteCmd.hpp"

InviteCmd::InviteCmd(){}

InviteCmd::InviteCmd(UserLst &user_lst, ChannelLst &chan_lst, const std::string &data)
	: ACommand(user_lst, chan_lst, data){}

InviteCmd::~InviteCmd(){}

void	InviteCmd::execute(int fd) 
{
	Client& sender = Client::getClientByFd(*_user_lst, fd);
	std::string user_name;
	std::string channel;

	std::istringstream iss(_data);
	std::getline(iss, user_name, ' ');
	std::getline(iss, channel);

	if (channel.empty() || user_name.empty()) {
		Messages::sendServMsg(fd, "INVITE :Not enough parameters", "461 " + sender.getNick()); return;}

	Channel* target_channel = Channel::getChannelByNamePt(*_chan_lst, channel);
	Client* invited_client = Client::getClientByNickPt(*_user_lst, user_name);

	if (!target_channel) {
		Messages::sendServMsg(fd, channel + " :No such channel", "403 " + sender.getNick()); return;}

	if (!Channel::isUserInChannel(*target_channel, sender)) {
		Messages::sendServMsg(fd, channel + " :You're not on that channel", "442 " + sender.getNick()); return;}

	if (!Channel::isOperatorInChannel(*target_channel, sender)) {
		Messages::sendServMsg(fd, channel + " :You're not channel operator", "482 " + sender.getNick()); return;}

	if (!invited_client){
		Messages::sendServMsg(fd, user_name + " :No such nick/channel", "401 " + sender.getNick()); return;}

	if (Channel::isUserInChannel(*target_channel, *invited_client)) {
		Messages::sendServMsg(fd, user_name + " " + channel + " :is already on channel", "443 " + sender.getNick()); return;}

	if (!Channel::isInvitedUserInChannel(*target_channel, *invited_client))
		target_channel->addInvitedUser(*invited_client);

	Messages::sendServMsg(fd, user_name + " " + target_channel->getName(), "341 " + sender.getNick());
	Messages::sendMsg(invited_client->getFd(), user_name + " " + target_channel->getName(), sender, "INVITE");
}
