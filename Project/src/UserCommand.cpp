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
	// Need to explore what user can contain so we check allowed chars. josegar2
	/////////////////////////////////////////////////////	Need to filter whats valid for User! josegar2
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
	///////////////////////////////////////////////////////////
	return true;
}

void UserCommand::execute(const Command& cmd, Client& sender) {
	(void) sender;
	const std::vector<std::string>& args = cmd.getArgs();
	// TODO: implement check for gettin cmd and/or sender as NULL
	if (args.empty()) {
		std::cout << "No arguments in the User command. Aborting."
			<< std::endl;
		return ;
	}
	const std::string& User = args[0]; // need to check the other args?? 
	if (UserCommand::isValidUser(User)) {
		sender.set_user(User);
		std::cout << "Executing User command. User: " << User << " assigned to client: " << sender.get_clientFD() << std::endl;
		
		
		///	testing 25.04.25 09.26 am
		std::string welcome_msg = ":" + sender.getServer()->getServerName() + " 001 " + sender.get_nick() + " :Welcome to the IRC Network, " + sender.get_nick() + "!" + sender.get_user() + "@" + "localhost" + "\r\n" + ":" + sender.getServer()->getServerName() + " 002 " + sender.get_nick() + " :Your host is " + sender.getServer()->getServerName() + ", running version 1.0" +"\r\n" + ":" + sender.getServer()->getServerName() + " 003 "+ sender.get_nick() + " :Text 3" +"\r\n" + ":" + sender.getServer()->getServerName() + " 004 " + sender.get_nick() + " " + sender.getServer()->getServerName() + " 1.0" +"\r\n" + ":" + sender.getServer()->getServerName() + " 375 " + sender.get_nick() + " :msg of the day" +"\r\n" + ":" + sender.getServer()->getServerName() +" 372 " + sender.get_nick() + " :- Welcome" +"\r\n" + ":" + sender.getServer()->getServerName() +" 376 " + sender.get_nick() + " :End of /MOTD command." +"\r\n";
		sender._out.addMessage(welcome_msg);
		sender.cl_Epoll_In_Out();
		///

	}
	else {
		return ;
	}
}
