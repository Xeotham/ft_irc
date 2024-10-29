#ifndef PARAMS_HPP
# define PARAMS_HPP

class Channel;
class Client;

# define SERVER_NAME std::string("IRC")

# include <vector>

typedef std::vector<Client> UserLst;
typedef std::vector<Channel> ChannelLst;

# include <iostream>
# include <sys/socket.h>
# include <sys/types.h>
# include <netinet/in.h>
# include <poll.h>
# include <fcntl.h>
# include <arpa/inet.h>
# include <unistd.h>
# include <map>
# include <csignal>
# include <limits>
# include <cstring>
# include <cstdlib>
# include <sstream>
# include <netdb.h>
# include <cstdlib>
# include <ctime>
# include "Messages.hpp"
# include "Channel.hpp"
# include "Client.hpp"


#endif //PARAMS_HPP
