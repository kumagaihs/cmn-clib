#ifndef _GREP_COMMAND_HPP
#define _GREP_COMMAND_HPP

#include "cmn-tools/Command.hpp"
extern "C" {
#include "cmnclib.h"
}

namespace cmnclib {
namespace tools {
namespace command {

class GrepCommand : public Command {

public:
	std::string name() override { return "grep"; }
	std::string summary() override  { return "指定ファイルもしくは標準入力の内容を条件に一致する行のみにフィルタリングします。"; }
	std::string help() override  { return summary() + "\n" + R"(
<<< command >>>
grep keyword [file-name...]

<<< option >>>
  keyword
        抽出条件文字列。この条件に一致しない行は無視される。
        keywordは正規表現（C++標準 regex_match）文字列として解釈する。

  file-name
        文字列検索対象のファイル。複数指定可能。
        省略した場合は標準入力をインプットとする。
)"; }

	int execute(std::vector<std::string>& args);

private:
	void output(const char *lineStr, const std::string &keyword, const std::string &fileName, int line);

};

}
}
}

#endif
