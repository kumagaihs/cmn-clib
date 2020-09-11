#include <iostream>
#include "cmn-tools/Command.hpp"
#include "cmn-tools/commands/HelpCommand.hpp"

namespace cmnclib {
namespace tools {
namespace command {

	int HelpCommand::execute(std::vector<std::string> args) {
		//TODO
		std::cout << "help";
		return 0;
	}

}
}
}