#include "ModeCmd.hpp"

ModeCmd::ModeCmd(){}

ModeCmd::ModeCmd(UserLst &user_lst, ChannelLst &chan_lst, const std::string &data)
	: ACommand(user_lst, chan_lst, data){}

ModeCmd::~ModeCmd(){}

void getNextWord(std::string &source, std::string &destination) {
	std::istringstream iss(source);
	iss >> destination;                
	std::getline(iss, source);
	if (source.find(' ') == std::string::npos)
		source.clear();
	else
		source.erase(0, source.find_first_not_of(" "));
}

void	ModeCmd::execute(int fd)
{
	Client& 			sender = Client::getClientByFd(*_user_lst, fd);
	std::string			channel;
	std::string			mode;
	std::string			last_param;
	std::string			arg;
	std::stringstream	ss;
	bool				enable;

	std::istringstream iss(_data);
    std::getline(iss, channel, ' ');
    std::getline(iss, mode, ' ');
    std::getline(iss, last_param);

	if (channel.empty()){
		Messages::sendMsg(fd, "MODE :Not enough parameters", sender, "461"); return ;}

	Channel* target_channel = Channel::getChannelByNamePt(*_chan_lst, channel);
	if (!target_channel){
		Messages::sendMsg(fd, channel + " :No such channel", sender, "403"); return ;}

	if (mode.empty() && last_param.empty() && target_channel->isModeSet('l')){
		ss << target_channel->getUserLimit();
		std::string limit = ss.str();
		Messages::sendMsg(fd, channel + " +" + target_channel->getModes() + " " + limit + " " + target_channel->getPassword(), sender, "324"); 
		return ;}
	else if (mode.empty()){
		Messages::sendMsg(fd, channel + " +" + target_channel->getModes() + " " + target_channel->getPassword(), sender, "324"); return ;}

	if (!Channel::isOperatorInChannel(*target_channel, sender)){
		Messages::sendMsg(fd, channel + " :You're not channel operator", sender, "482"); return ;}

	if (mode[0] == '-')
		enable = false;
	else
		enable = true;

	if (mode[0] == '-' || mode[0] == '+')
		mode.erase(0, 1);

	getNextWord(last_param, arg);

std::string msggg = "mamama you changed the t mode\r\n";
	for (std::string::size_type i = 0; i != mode.size(); ++i){
		char	c = mode[i];
		
		if (!target_channel->isExistingMode(c)) {
			Messages::sendMsg(fd, std::string(1, c) + " :is unknown mode char to me", sender, "472"); continue;}

		switch (c){
			case 'i':
				target_channel->setMode(c, enable);
				break;
			case 't':
				send(fd, msggg.c_str(), msggg.size(), 0);
				target_channel->setMode(c, enable);
				break;
			case 'k':
				kMode(target_channel, enable, arg);
				break ;
			case 'o':
				oMode(target_channel, enable, arg, fd, sender);
				break ;
			case 'l':
				lMode(target_channel, enable, arg);
		}

		if (c == 'k' || c == 'o' || c == 'l'){
			if (!last_param.empty())
				getNextWord(last_param, arg);
			else
				arg.clear();
		}
	}

}

void	ModeCmd::kMode(Channel *target_channel, const bool enable, const std::string arg){
	if (!enable){
		target_channel->setPassword(""); 
		target_channel->setMode('k', enable); 
		return;}

	if (arg.empty())
		return ;

	target_channel->setPassword(arg);
	target_channel->setMode('k', enable);
}

void	ModeCmd::oMode(Channel *target_channel, const bool enable, std::string arg, int fd, Client &sender){
	if (!Client::isClientInList(target_channel->getUsers(), arg)){
		Messages::sendMsg(fd, arg + " :No such nick/channel", sender, "401"); 
		target_channel->setMode('o', enable); 
		return;}
	
	Client& target_client = Client::getClientByNick(target_channel->getUsers(), arg);
	if (!enable){
		target_channel->removeOperator(target_client); return;}
	target_channel->addOperator(target_client); // Should I catch the throw or it's useless?
	target_channel->setMode('o', enable); 
}

void	ModeCmd::lMode(Channel *target_channel, const bool enable, const std::string arg){	
	if (!enable){
		target_channel->setUserLimit(0); 
		target_channel->setMode('l', enable); 
		return ;}

	if (arg.empty())
		return ;

	std::istringstream iss(arg);
	unsigned int limit;
	if (!(iss >> limit) || !iss.eof())
		return ;

	target_channel->setUserLimit(limit);
	target_channel->setMode('l', enable); 
}

