#include <iostream>
#include <iomanip>
#include "cmn-tools/Command.hpp"
#include "cmn-tools/commands/HelpCommand.hpp"

namespace cmnclib {
namespace tools {
namespace command {

	int HelpCommand::execute(std::vector<std::string>& args) {
		// コマンド名の指定がない場合はコマンド一覧を表示
		if (args.size() == 0) {
			std::cout << "How to use  : cmn-tools subcommand [option ...]" << std::endl;
			std::cout << "Help details: cmn-tools help subcommand" << std::endl << std::endl;

			std::cout << "<Subcommands>" << std::endl;
			for (Command *cmd : Command::getAllCommands()) {
				std::cout << std::left << std::setw(16) << cmd->name() << " ";
				std::cout << cmd->summary() << std::endl;
			}
		}
		// コマンド名の指定がある場合はコマンド詳細を表示
		else {
			Command *cmd = Command::getCommand(args[0]);
			std::cout << cmd->help() << std::endl;
		}

		return 0;
	}

}
}
}