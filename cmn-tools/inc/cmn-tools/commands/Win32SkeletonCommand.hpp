#ifndef _WIN32SKELETON_COMMAND_HPP
#define _WIN32SKELETON_COMMAND_HPP

#include "cmn-tools/Command.hpp"

namespace cmnclib {
namespace tools {
namespace command {

class Win32SkeletonCommand : public Command {

public:
	std::string name() override { return "win32skeleton"; }
	std::string summary() override  { return "WIN32のスケルトンウィンドウを表示します。"; }
	std::string help() override  { return summary() + "\n" + R"(
<<< command >>>
win32skeleton

<<< option >>>
なし
)"; }

	int execute(std::vector<std::string>& args);

};

}
}
}

#endif
