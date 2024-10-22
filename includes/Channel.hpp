#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <Params.hpp>
# include <map>
# include <Client.hpp>

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
		void					addUser(Client& user);
		void                    addOperator(Client& user);
		void					removeUser(const Client& user);
		UserLst					&getUsers();
		const std::string		&getName() const;
		const std::string		&getPassword() const;
	public:
		static Channel	&getChannelByName(ChannelLst &lst, const std::string &name);
		static bool		isUserInChannel(Channel &channel, const Client &user);
	private:
		Channel();
	private:
		std::string	_name;
		std::string	_password;
		UserLst		_operators;
		UserLst		_users;
};

#endif // CHANNEL_HPP