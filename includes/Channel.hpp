#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <Params.hpp>
# include <map>
# include <Client.hpp>
# include <set>

class Channel {
	public:
		Channel(const std::string& name);
		Channel(const std::string &name, const std::string &pwd);
		Channel(const Channel& other);
		~Channel();
    public:
		Channel& operator=(const Channel& other);
	public:
		void					setName(const std::string& name);
		void					setTopic(const std::string& topic);
		void					setMode(const char mode, const bool enable);
		void 					setUserLimit(unsigned int limit);
		void					setPassword(const std::string &password);
		void					addUser(Client user);
		void                    addOperator(Client user);
		void					addInvitedUser(Client user);
		void					removeUser(const Client& user);
		void					removeOperator(const Client& user);
		UserLst					&getUsers();
		UserLst					&getOperators();
		const std::string		&getName() const;
		const std::string		&getPassword() const;
		const std::string		&getTopic() const;
		const std::string		getModes() const;
		unsigned int			getUserLimit() const;
		bool					isModeSet(const char mode) const;
		bool					isExistingMode(const char c) const;
	public:
		static Channel	&getChannelByName(ChannelLst &lst, const std::string &name);
		static Channel	*getChannelByNamePt(ChannelLst &lst, const std::string &name);
		static bool		isUserInChannel(Channel &channel, const Client &user);
		static bool		isOperatorInChannel(Channel &channel, const Client &user);
		static bool		isInvitedUserInChannel(Channel &channel, const Client &user);
		static void		removeChannelFromLst(ChannelLst &channels_lst, const Channel &channel);
	private:
		Channel();
	private:
		std::string		_name;
		std::string		_password;
		std::string		_topic;
		std::set<char>	_modes;
		UserLst			_invited_users;
		UserLst			_operators;
		UserLst			_users;
		unsigned int	_user_limit;
};

#endif // CHANNEL_HPP