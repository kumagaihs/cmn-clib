#ifndef _HELP_COMMAND_HPP
#define _HELP_COMMAND_HPP

#include "cmn-tools/Command.hpp"

namespace cmnclib {
namespace tools {
namespace command {

class HelpCommand : public Command {

public:
	std::string name() override { return "help"; }
	std::string summary() override { return "�w���v��W���o�͂ɏo�͂��܂��B"; }
	std::string help() override { return summary() + "\n" + R"(
<<< command >>>
help [command-name]

<<< option >>>
  command-name
        �ڍׂ�m�肽���R�}���h���B�ȗ������ꍇ�̓R�}���h�̈ꗗ��\������
)";
	}

	int execute(std::vector<std::string> args);

};

}
}
}

#endif
