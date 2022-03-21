#ifndef _TAIL_COMMAND_HPP
#define _TAIL_COMMAND_HPP

#include "cmn-tools/Command.hpp"

namespace cmnclib {
namespace tools {
namespace command {

class TailCommand : public Command {

public:
	std::string name() override { return "tail"; }
	std::string summary() override  { return "ファイルの末尾を表示する"; }
	std::string help() override  { return summary() + "\n" + R"(
<<< command >>>
（注）未実装
tail [-n lines] [-f] target

指定ファイルの末尾を表示する。

<<< option >>>
  -n
        表示する行数を指定する。省略した場合は10行表示となる

  -f
        ファイルを監視し更新された内容を表示する

  target
        対象ファイル
)"; }

	int execute(std::vector<std::string>& args);

};

}
}
}

#endif
