#include <Bot.hpp>

Bot::Bot() {}
Bot::Bot(const Bot& other) {(void)other;}
Bot& Bot::operator=(const Bot& other) {(void)other;	return *this;}
Bot::~Bot() {}

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

void    Bot::botCommand(int fd, std::string data, std::vector<Client> &vec)
{
	int i = 0;
	size_t pos = 0;
	Client &user = Client::getClientByFd(vec, fd);
	std::string cmd[] = {"cmd\r\n", "joke\r\n", "ping\r\n"};
	t_func func[] = {&Bot::sendCommand, &Bot::sendJoke, &Bot::sendPong};
	pos = data.find(' ');
	data.erase(0, pos + 1);
	std::cout << data << std::endl;
	while(i < 3 && cmd[i].compare(data))
		i++;
	if (i < 3)
		(*func[i])(fd, user);
}
