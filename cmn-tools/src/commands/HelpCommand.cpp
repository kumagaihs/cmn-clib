#include <iostream>
#include <iomanip>
#include "cmn-tools/Command.hpp"
#include "cmn-tools/commands/HelpCommand.hpp"

namespace cmnclib {
namespace tools {
namespace command {

	int HelpCommand::execute(std::vector<std::string>& args) {
		// �R�}���h���̎w�肪�Ȃ��ꍇ�̓R�}���h�ꗗ��\��
		if (args.size() == 0) {
			std::cout << "How to use  : cmn-tools subcommand [option ...]" << std::endl;
			std::cout << "Help details: cmn-tools help subcommand" << std::endl << std::endl;

			std::cout << "<Subcommands>" << std::endl;
			for (Command *cmd : Command::getAllCommands()) {
				std::cout << std::left << std::setw(16) << cmd->name() << " ";
				std::cout << cmd->summary() << std::endl;
			}
		}
		// �R�}���h���̎w�肪����ꍇ�̓R�}���h�ڍׂ�\��
		else {
			Command *cmd = Command::getCommand(args[0]);
			std::cout << cmd->help() << std::endl;
		}

		return 0;
	}

}
}
}