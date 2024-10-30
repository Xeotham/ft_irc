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
	bool				enable = true;

	std::istringstream iss(_data);
    std::getline(iss, channel, ' ');
    std::getline(iss, mode, ' ');
    std::getline(iss, last_param);

	if (channel.empty()){
		Messages::sendServMsg(fd, "MODE :Not enough parameters", "461 " + sender.getNick()); return ;}

	Channel* target_channel = Channel::getChannelByNamePt(*_chan_lst, channel);
	if (!target_channel){
		Messages::sendServMsg(fd, channel + " :No such channel", "403 " + sender.getNick()); return ;}

	if (mode.empty() && last_param.empty() && target_channel->isModeSet('l')){
		ss << target_channel->getUserLimit();
		std::string limit = ss.str();
		Messages::sendServMsg(fd, channel + " +" + target_channel->getModes() + " " + limit + " " + target_channel->getPassword(), "324 " + sender.getNick()); 
		return ;}
	else if (mode.empty()){
		Messages::sendServMsg(fd, channel + " +" + target_channel->getModes() + " " + target_channel->getPassword(), "324 " + sender.getNick()); return ;}

	if (!Channel::isOperatorInChannel(*target_channel, sender)){
		Messages::sendServMsg(fd, channel + " :You're not channel operator", "482 " + sender.getNick()); return ;}

	getNextWord(last_param, arg);

	std::cout << "THE MODE LINE IS: " << mode << std::endl;


	for (std::string::size_type i = 0; i != mode.size(); ++i){
		char	c = mode[i];
		
		if (!target_channel->isExistingMode(c)) {
			Messages::sendServMsg(fd, std::string(1, c) + " :is unknown mode char to me", "472 " + sender.getNick()); continue;}

		switch (c){
			case 'i':
				iMode(target_channel, enable, sender);
				break;
			case 't':
				tMode(target_channel, enable, sender);
				break;
			case 'k':
				kMode(target_channel, enable, arg, sender);
				break ;
			case 'o':
				oMode(target_channel, enable, arg, fd, sender);
				break ;
			case 'l':
				lMode(target_channel, enable, arg, sender);
				break ;
			case '+' :
				enable = true;
				break ;
			case '-' :
				enable = false;
		}

		if (c == 'k' || c == 'o' || c == 'l'){
			if (!last_param.empty())
				getNextWord(last_param, arg);
			else
				arg.clear();
		}
	}
}

void	ModeCmd::iMode(Channel *target_channel, const bool enable, Client &sender){
	if (!enable)
		Messages::sendGlobalMsg(target_channel->getUsers(), target_channel->getName() + " -i", sender, "MODE");	
	else
		Messages::sendGlobalMsg(target_channel->getUsers(), target_channel->getName() + " +i", sender, "MODE");
	target_channel->setMode('i', enable);
}

void	ModeCmd::tMode(Channel *target_channel, const bool enable, Client &sender){
	if (!enable)
		Messages::sendGlobalMsg(target_channel->getUsers(), target_channel->getName() + " -t", sender, "MODE");	
	else
		Messages::sendGlobalMsg(target_channel->getUsers(), target_channel->getName() + " +t", sender, "MODE");
	target_channel->setMode('t', enable);
}

void	ModeCmd::kMode(Channel *target_channel, const bool enable, const std::string arg, Client &sender){
	if (!enable){
		target_channel->setPassword(""); 
		target_channel->setMode('k', enable); 
		Messages::sendGlobalMsg(target_channel->getUsers(), target_channel->getName() + " -k", sender, "MODE");
		return;}
	if (arg.empty())
		return ;

	target_channel->setPassword(arg);
	target_channel->setMode('k', enable);
	Messages::sendGlobalMsg(target_channel->getUsers(), target_channel->getName() + " +k " + arg, sender, "MODE");
}

void	ModeCmd::oMode(Channel *target_channel, const bool enable, std::string arg, int fd, Client &sender){	
	
	Client* target_client = Client::getClientByNick(target_channel->getUsers(), arg);
	if (!target_channel){
		Messages::sendServMsg(fd, arg + " :No such nick/channel", "401 " + sender.getNick()); 
		return;}

	if (!enable){
		target_channel->removeOperator(*target_client); 
		target_channel->setMode('o', enable); 
		Messages::sendGlobalMsg(target_channel->getUsers(), target_channel->getName() + " -o " + target_client->getNick(), sender, "MODE");		
		return;}
	target_channel->addOperator(*target_client); // Should I catch the throw or it's useless?
	target_channel->setMode('o', enable);
	Messages::sendGlobalMsg(target_channel->getUsers(), target_channel->getName() + " +o " + target_client->getNick(), sender, "MODE");
}

void	ModeCmd::lMode(Channel *target_channel, const bool enable, const std::string arg, Client &sender){	
	if (!enable){
		target_channel->setUserLimit(0); 
		target_channel->setMode('l', enable); 
		Messages::sendGlobalMsg(target_channel->getUsers(), target_channel->getName() + " -l", sender, "MODE");
		return ;}

	if (arg.empty())
		return ;

	std::istringstream iss(arg);
	unsigned int limit;
	if (!(iss >> limit) || !iss.eof())
		return ;

	target_channel->setUserLimit(limit);
	target_channel->setMode('l', enable);
	Messages::sendGlobalMsg(target_channel->getUsers(), target_channel->getName() + " +l " + arg, sender, "MODE");
}
