#ifndef _HELP_COMMAND_HPP
#define _HELP_COMMAND_HPP

#include "cmn-tools/Command.hpp"

namespace cmnclib {
namespace tools {
namespace command {

class HelpCommand : public Command {

public:
	std::string name() override { return "help"; }
	std::string summary() override { return "ヘルプを標準出力に出力します。"; }
	std::string help() override { return summary() + "\n" + R"(
<<< command >>>
help [command-name]

<<< option >>>
  command-name
        詳細を知りたいコマンド名。省略した場合はコマンドの一覧を表示する
)";
	}

	int execute(std::vector<std::string> args);

};

}
}
}

#endif
