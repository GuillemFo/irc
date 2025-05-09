# include "UserCommand.hpp"
# include <iostream>

UserCommand::UserCommand() : _server(NULL) {}
UserCommand::UserCommand(const UserCommand& src) {
	this->_server = src._server;
}
UserCommand& UserCommand::operator=(const UserCommand& src) {
	if (this != &src) {
		this->_server = src._server;
	}
	return *this;
}

UserCommand::~UserCommand () {}
UserCommand::UserCommand (Server* server) : _server(server) {}

bool UserCommand::isValidUser(const std::string& name) {
	if (name.empty()) {
		std::cout << "Aborting User: User is empty."
			<< std::endl;
		return false;
	}
	// Need to explore what user can contain so we check allowed chars. josegar2 TODO
	if (name.length() > 50) {
		std::cout << "Aborting USER: Username contains over 50"
			<< " symbols." << std::endl;
		return false;
	}
	for (size_t i = 0; i < name.length(); ++i) {
		char c = name[i];
		if (c == ' ' || c == ',' || c < 32) {
			std::cout << "Aborting USER: Username contains invalid"
				<< " symbols." << std::endl;
			return false;
		}
	}
	//
	return true;
}

void UserCommand::execute(const Command& cmd, Client& sender) {
	(void) sender;
	const std::vector<std::string>& args = cmd.getArgs();
	// TODO: implement check for gettin cmd and/or sender as NULL
	if (sender.getOkLogin()) { // if already full registered
		sender.sendMessage(ircErrorText(ERR_ALREADYREGISTERED, cmd, sender));
		return ;
	}
	if (args.size() < 4) {
		sender.sendMessage(ircErrorText(ERR_NEEDMOREPARAMS, cmd, sender));
		return ;
	}
	if (sender.isRegistered())
	{
		const std::string& User = args[0]; // need to check the other args?? 
		if (UserCommand::isValidUser(User)) {
			sender.set_user(User);
			sender.setRealName(args[3]);
			sender.setOkLogin();
			std::cout << "Executing User command. User: " << User << " assigned to client: " << sender.get_clientFD() << std::endl;
			sender.sendMessage(ircReplyText(RPL_WELCOME, cmd, sender));
			sender.sendMessage(ircReplyText(RPL_YOURHOST, cmd, sender));
			sender.sendMessage(ircReplyText(RPL_MOTD, cmd, sender));
			//sender.sendMessage(ircReplyText(RPL_ENDOFMOTD, cmd, sender)); //we can skip this since it impies we have /MOTD command
			return ;
			///
		}
	}
	else {
		sender.sendMessage(ircErrorText(ERR_NONICKNAMEGIVEN, cmd, sender));
		return ;
	}
}
