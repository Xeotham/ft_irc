#include <Bot.hpp>

Bot::Bot() {}
Bot::Bot(const Bot& other) {(void)other;}
Bot& Bot::operator=(const Bot& other) {(void)other;	return *this;}
Bot::~Bot() {}

Bot::Bot(UserLst &user_lst, ChannelLst &chan_lst, const std::string &data) : ACommand(user_lst, chan_lst, data){}


void    Bot::sendCommand(int fd, Client &user)
{
	std::string message = ".\r\n";
	(void)user;
	send(fd, message.c_str(), message.size(), 0);
}

void    Bot::sendJoke(int fd, Client &user)
{
	std::string joke[] = {" How do trees get on the Internet ?\r\n", " What is a computer’s first sign of old age ?\r\n",
						  " What does a baby computer call his father ?\r\n", " What is an astronaut’s favorite control on the computer keyboard ?\r\n",
						  " What happened when the computer fell on the floor ?\r\n", " Why was there a bug in the computer?\r\n",
						  " What is a computer virus ?\r\n", " Have you heard about the Disney virus?\r\n",
						  " What happened when a dragon breathed on several Macintosh computers ?\r\n"};
	std::string answer[] = {" They log in.\n\r", " Loss of memory.\r\n", " Da-ta.\r\n", " The space bar.\r\n",
							" It slipped a disk.\r\n", " It was looking for a byte to eat.\r\n", " A terminal illness.\r\n",
							" It makes everything on your computer go Goofy.\r\n", " He wound up with baked Apples !\r\n"};
	int i = std::rand() % 9;
	Messages::sendMsg(fd, user.getNick() + joke[i], BOT, MSG);
	Messages::sendMsg(fd, user.getNick() + answer[i], BOT, MSG);
}

void    Bot::sendPong(int fd, Client &user)
{
	Messages::sendMsg(fd, user.getNick() + " pong", BOT, MSG);
}

void    Bot::execute(int fd)
{
	int i = 0;
	// size_t pos = 0;
	Client &user = Client::getClientByFd(*_user_lst, fd);
	std::string cmd[] = {"cmd", "joke", "ping"};
	std::cout << _data << std::endl;
	while(i < 3 && cmd[i].compare(_data))
		i++;
	switch (i) {
		case 0:
			sendCommand(fd, user);
			break;
		case 1:
			sendJoke(fd, user);
			break;
		case 2:
			sendPong(fd, user);
			break;
	}
}
