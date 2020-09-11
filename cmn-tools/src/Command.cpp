#include "cmn-tools/Command.hpp"

namespace cmnclib {
namespace tools {
namespace command {

	std::vector<Command*> Command::m_allCommmands;

	std::vector<Command*>& Command::getAllCommands() {
		return m_allCommmands;
	}

	Command* Command::getCommand(const std::string& name) {
		for (Command *cmd : m_allCommmands) {
			if (cmd->name().compare(name) == 0) {
				return cmd;
			}
		}
		throw CommandException(name, __FILE__, __LINE__, "command not found");
	}

	void Command::registCommand(Command *command) {
		m_allCommmands.push_back(command);
	}

}
}
}