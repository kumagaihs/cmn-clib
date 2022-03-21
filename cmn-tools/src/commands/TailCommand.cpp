#include <iostream>
#include <sstream>

#include "cmn-tools/Command.hpp"
#include "cmn-tools/CommandException.hpp"
#include "cmn-tools/commands/TailCommand.hpp"

extern "C" {
#include "cmnclib.h"
}

namespace cmnclib {
namespace tools {
namespace command {

	int TailCommand::execute(std::vector<std::string>& args) {
		if (args.size() < 1) {
			throw CommandException(this->name(), __FILE__, __LINE__, "File path required.");
		}

		boolean flag_f;

		//TODO ŽÀ‘•

		return 0;
	}

}
}
}