#ifndef ERROR_HPP
# define ERROR_HPP

# include "Params.hpp"

# define ERR_NOSUCHNICK			"401"
# define ERR_NOSUCHCHANNEL		"403"
# define ERR_CANNOTSENDTOCHAN	"404"
# define ERR_TOOMANYTARGETS		"407"
# define ERR_NOORIGIN			"409"
# define ERR_NORECIPIENT		"411"
# define ERR_NOTEXTTOSEND		"412"
# define ERR_UNKNOWNCOMMAND		"421"
# define ERR_NONICKNAMEGIVEN	"431"
# define ERR_ERRONEUSNICKNAME	"432"
# define ERR_NICKNAMEINUSE		"433"
# define ERR_NOTONCHANNEL		"442"
# define ERR_NEEDMOREPARAMS		"461"
# define ERR_ALREADYREGISTRED	"462"
# define ERR_CHANNELISFULL		"471"
# define ERR_INVITEONLYCHAN		"473"
# define ERR_BADCHANNELKEY		"475"

# define NEEDMOREPARAMS_MSG(CMD)			(std::string(CMD) + " :Not enough parameters")
# define ALREADYREGISTRED_MSG				":You may not reregister"
# define NONICKNAMEGIVEN_MSG				":No nickname given"
# define NICKNAMEINUSE_MSG(NICK)			(std::string(NICK) + " :Nickname is already in use")
# define ERRONEUSNICKNAME_MSG(NICK)			(std::string(NICK) + " :Erroneus nickname")
# define NORECIPIENT_MSG(CMD)				(":No recipient given (" + std::string(CMD) + ")")
# define NOSUCHNICK_MSG(NICK)				(std::string(NICK) + " :No such nick/channel")
# define NOSUCHCHANNEL_MSG(CHANNEL)			(std::string(CHANNEL) + " :No such channel")
# define CANNOTSENDTOCHAN_MSG(CHANNEL)		(std::string(CHANNEL) + " :Cannot send to channel")
# define NOTEXTTOSEND_MSG					":No text to send"
# define TOOMANYTARGETS_MSG					":Too many recipients"
# define NOTONCHANNEL_MSG(CHANNEL)			(std::string(CHANNEL) + " :You're not on that channel")
# define UNKNOWNCOMMAND_MSG(CMD)			(std::string(CMD) + " :Unknown command")
# define NOORIGIN_MSG						":No origin specified"
# define ERR_CHANNELISFULL_MSG(CHANNEL)		(std::string(CHANNEL) + " :Cannot join channel (+l)")
# define ERR_INVITEONLYCHAN_MSG(CHANNEL)	(std::string(CHANNEL) + " :Cannot join channel (+i)")		
# define BADCHANNELKEY_MSG(CHANNEL)			(std::string(CHANNEL) + " :Cannot join channel (+k)")

class Error {
	public:
		Error(int fd, const Client &sender, const std::string &type, const std::string &msg);
		Error(const Error &other);
		~Error();
	public:
		Error &operator=(const Error &other);
	public:
		void	sendError() const;
		const std::string	&getType() const;
	private:
		Error();
	private:
		const Client		&_sender;
		const int			_fd;
		const std::string	_type;
		const std::string	_msg;
};

#endif //ERROR_HPP
