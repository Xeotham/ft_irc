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
		Messages::sendServMsg(fd, "TOPIC :Not enough parameters", "461 " + sender.getNick()); return ;}

	Channel* target_channel = Channel::getChannelByNamePt(*_chan_lst, channel);
	if (!target_channel){
		Messages::sendServMsg(fd, channel + " :No such channel", "403 " + sender.getNick()); return ;}

	if (!Channel::isUserInChannel(*target_channel, sender)){
		Messages::sendServMsg(fd, channel + " :You're not on that channel", "442 " + sender.getNick()); return ;}

	if (topic.empty() && target_channel->getTopic().empty()){
		Messages::sendServMsg(fd, channel + " :No topic is set", "331 " + sender.getNick()); return ;}

	if (topic.empty()){
		Messages::sendServMsg(fd, channel + " " + target_channel->getTopic(), "332 " + sender.getNick()); return ;}

	if (!Channel::isOperatorInChannel(*target_channel, sender) && target_channel->isModeSet('t')){
		Messages::sendServMsg(fd, channel + " :You're not channel operator", "482 " + sender.getNick()); return ;}

	target_channel->setTopic(topic);
	std::string msg = channel + " " + topic;
	Messages::sendGlobalMsg(target_channel->getUsers(), msg, sender, "TOPIC");
}
