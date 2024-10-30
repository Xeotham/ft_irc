#include "TopicCmd.hpp"

TopicCmd::TopicCmd(){}

TopicCmd::TopicCmd(UserLst &user_lst, ChannelLst &chan_lst, const std::string &data)
	: ACommand(user_lst, chan_lst, data){}

TopicCmd::~TopicCmd(){}

void	TopicCmd::execute(int fd)
{
	Client& 		sender = Client::getClientByFd(*_user_lst, fd);
	std::string		channel;
	std::string		topic;

	std::istringstream iss(_data);
    std::getline(iss, channel, ' ');
    std::getline(iss, topic);

	if (channel.empty()){
		Messages::sendMsg(fd, "TOPIC :Not enough parameters", sender, "461"); return ;}

	Channel* target_channel = Channel::getChannelByNamePt(*_chan_lst, channel);
	if (!target_channel){
		Messages::sendMsg(fd, channel + " :No such channel", sender, "403"); return ;}

	if (!Channel::isUserInChannel(*target_channel, sender)){
		Messages::sendMsg(fd, channel + " :You're not on that channel", sender, "442"); return ;}

	if (topic.empty() && target_channel->getTopic().empty()){
		Messages::sendMsg(fd, channel + " :No topic is set", sender, "331"); return ;}

	if (topic.empty()){
		Messages::sendMsg(fd, channel + " " + target_channel->getTopic(), sender, "332"); return ;}

	if (!Channel::isOperatorInChannel(*target_channel, sender)){
		Messages::sendMsg(fd, channel + " :You're not channel operator", sender, "482"); return ;}

	target_channel->setTopic(topic);
}
