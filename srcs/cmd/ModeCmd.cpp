#include "ModeCmd.hpp"

ModeCmd::ModeCmd(){}

ModeCmd::ModeCmd(UserLst &user_lst, ChannelLst &chan_lst, const std::string &data)
	: ACommand(user_lst, chan_lst, data){}

ModeCmd::~ModeCmd(){}

void	ModeCmd::execute(int fd)
{
	Client& 	sender = Client::getClientByFd(*_user_lst, fd);
	std::string	channel;
	std::string	mode;
	std::string	last_param;

	std::istringstream iss(_data);
    std::getline(iss, channel, ' ');
    std::getline(iss, mode, ' ');
    std::getline(iss, last_param, ' ');

	if (channel.empty() || mode.size() != 2){
		Messages::sendMsg(fd, "MODE :Not enough parameters", sender, "461"); return ;}

	Channel* target_channel = Channel::getChannelByNamePt(*_chan_lst, channel);
	if (!target_channel){
		Messages::sendMsg(fd, channel + " :No such channel", sender, "403"); return ;}

	if (mode.empty() && last_param.empty()){
		Messages::sendMsg(fd, channel + " +" + target_channel->getModes(), sender, "324"); return ;}

	if (!Channel::isOperatorInChannel(*target_channel, sender)){
		Messages::sendMsg(fd, channel + " :You're not channel operator", sender, "482"); return ;}

	if (!target_channel->isExistingMode(mode[1])){
		Messages::sendMsg(fd, mode[1] + " :is unknown mode char to me", sender, "472"); return ;}


	// if no mode -> print 


}
