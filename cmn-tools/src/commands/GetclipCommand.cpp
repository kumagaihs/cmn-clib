#include <iostream>

#include "cmn-tools/Command.hpp"
#include "cmn-tools/CommandException.hpp"
#include "cmn-tools/commands/GetclipCommand.hpp"

extern "C" {
#include "cmnclib.h"
}

namespace cmnclib {
namespace tools {
namespace command {

	int GetclipCommand::execute(std::vector<std::string>& args) {
		CmnStringBuffer *buf = CmnStringBuffer_Create("");
		if (CmnWin32Clipboard_GetString(buf) < 0) {
			throw CommandException(this->name(), __FILE__, __LINE__, "Cannot open the Clipboard");
		}
		std::cout << buf->string;
		return 0;
	}

}
}
}