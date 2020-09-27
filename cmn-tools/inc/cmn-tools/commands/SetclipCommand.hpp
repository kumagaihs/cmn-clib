#ifndef _SETCLIP_COMMAND_HPP
#define _SETCLIP_COMMAND_HPP

#include "cmn-tools/Command.hpp"

namespace cmnclib {
namespace tools {
namespace command {

class SetclipCommand : public Command {

public:
	std::string name() override  { return "setclip"; }
	std::string summary() override  { return "�W�����͂���ǂݍ��񂾃f�[�^���N���b�v�{�[�h�ɏo�͂��܂��B"; }
	std::string help() override  { return summary() + "\n" + R"(
<<< command >>>
setclip

<<< option >>>
�Ȃ�
)";
	}

	int execute(std::vector<std::string>& args);

};

}
}
}

#endif
