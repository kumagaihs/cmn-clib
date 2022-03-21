#ifndef _XARGS_COMMAND_HPP
#define _XARGS_COMMAND_HPP

#include "cmn-tools/Command.hpp"

namespace cmnclib {
namespace tools {
namespace command {

class XargsCommand : public Command {

public:
	std::string name() override { return "xargs"; }
	std::string summary() override  { return "標準入力のデータで指定のコマンドを繰り返し実行します。"; }
	std::string help() override  { return summary() + "\n" + R"(
<<< command >>>
（注）実装中
xargs [-s] command [command-options...]

標準入力から一行読み取り、読み取ったデータを最後の引数に付与してcommandを実行する。
これを標準入力のデータがなくなるまで繰り返し実行する。

<<< option >>>
  -s
        サブシェルで実行する。パイプやリダイレクト等のシェル機能を利用可能とする。

  command
        実行するコマンド

  command-options
        commandに付与する引数。複数指定可能。

)"; }

	int execute(std::vector<std::string>& args);

};

}
}
}

#endif
