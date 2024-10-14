#include "Bot.hpp"

Bot::Bot() 
{
}

Bot::Bot(const Bot& other) : Client(other)
{
}

Bot& Bot::operator=(const Bot& other) {
	if (this != &other) 
	{
		Client::operator=(other);
	}
	return *this;
}

Bot::~Bot()
{
}

bool	sendToServer(int sockfd, const std::string& msg) {
	if (send(sockfd, msg.c_str(), msg.size(), 0) < 0)
	{
		std::cerr << "Failed to send message" << std::endl;
		return false;
	}
	return true;
}

void Bot::initBot(const std::string& server, int port, int &sockfd, std::string password) {
	if (connectToServer(server, port, sockfd) == true) 
	{
		if (sendToServer(sockfd, "PASS " + password + "\r\n") && sendToServer(sockfd, "NICK bot\r\n") && sendToServer(sockfd, "USER botname 0 * bot\r\n")) {
			std::cout << "Connected to server" << std::endl;
			return ;
		}
	}
	close(sockfd);
	std::cerr << "Failed to connect to server" << std::endl;
}

bool Bot::connectToServer(const std::string& server, int port, int &sockfd)
{
	struct hostent *host;

	if ((host = gethostbyname(server.c_str())) == NULL) {
		std::cerr << "Failed to get host by" << std::endl;
		return false;
	}
	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		std::cerr << "Error creating socket" << std::endl;
		return false;
	}

	struct sockaddr_in server_addr;
	std::memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);

	if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
		std::cerr << "Connection Failed" << std::endl;
		return false;
	}
	return true;
}