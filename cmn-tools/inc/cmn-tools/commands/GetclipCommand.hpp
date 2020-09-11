#ifndef _GETCLIP_COMMAND_HPP
#define _GETCLIP_COMMAND_HPP

#include "cmn-tools/Command.hpp"

namespace cmnclib {
namespace tools {
namespace command {

class GetclipCommand : public Command {

public:
	std::string name() override { return "getclip"; }
	std::string summary() override  { return "�N���b�v�{�[�h�̃f�[�^��W���o�͂ɏo�͂��܂��B"; }
	std::string help() override  { return summary() + "\n" + R"(
<<< command >>>
getclip

<<< option >>>
�Ȃ�
)"; }

	int execute(std::vector<std::string> args);

};

}
}
}

#endif
