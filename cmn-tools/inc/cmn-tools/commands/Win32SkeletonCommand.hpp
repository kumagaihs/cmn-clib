#ifndef _WIN32SKELETON_COMMAND_HPP
#define _WIN32SKELETON_COMMAND_HPP

#include "cmn-tools/Command.hpp"

namespace cmnclib {
namespace tools {
namespace command {

class Win32SkeletonCommand : public Command {

public:
	std::string name() override { return "win32skeleton"; }
	std::string summary() override  { return "WIN32�̃X�P���g���E�B���h�E��\�����܂��B"; }
	std::string help() override  { return summary() + "\n" + R"(
<<< command >>>
win32skeleton

<<< option >>>
�Ȃ�
)"; }

	int execute(std::vector<std::string>& args);

};

}
}
}

#endif
