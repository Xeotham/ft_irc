#include "Server.hpp"

int	main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cerr << "Error : Usage ./ircserv <port> <password>" << std::endl;
		return 1;
	}
	std::string argv1 = argv[1];
	if (argv1.find_first_not_of("0123456789") != std::string::npos || std::atof(argv1.c_str()) > std::numeric_limits<int>::max())
	{
		std::cerr << "Error : <port> must be a interger value" << std::endl;
		return 1;
	}
	
	Server serv(std::atoi(argv[1]), argv[2]);
	try
	{
		signal(SIGPIPE, SIG_IGN);
		signal(SIGINT, Server::signalHandler);
		signal(SIGQUIT, Server::signalHandler);
		serv.serverInit();
	}
	catch(const std::exception& e)
	{
		serv.closeFds();
		std::cerr << e.what() << std::endl;
	}

	return 0;
}