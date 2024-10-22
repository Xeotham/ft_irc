#ifndef PARAMS_HPP
# define PARAMS_HPP

# include <iostream>
# include <sys/socket.h>
# include <sys/types.h>
# include <netinet/in.h>
# include <poll.h>
# include <fcntl.h>
# include <arpa/inet.h>
# include <unistd.h>
# include <vector>
# include <map>
# include <csignal>
# include <limits>
# include <cstring>
# include <cstdlib>
# include <sstream>
# include <netdb.h>
# include <cstdlib>
# include <ctime>
#include "Messages.hpp"

class Channel;
class Client;

typedef std::vector<Client> UserLst;
typedef std::vector<Channel> ChannelLst;

#endif //PARAMS_HPP
