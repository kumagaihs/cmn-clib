#include <iostream>
#include <sstream>

#include "cmn-tools/Command.hpp"
#include "cmn-tools/CommandException.hpp"
#include "cmn-tools/commands/XargsCommand.hpp"

extern "C" {
#include "cmnclib.h"
}

namespace cmnclib {
namespace tools {
namespace command {

	int XargsCommand::execute(std::vector<std::string>& args) {
		if (args.size() < 1) {
			throw CommandException(this->name(), __FILE__, __LINE__, "command required.");
		}

		// シェル解釈ありで実行（system実行）
		if (args[0] == "-s") {
			// 引数チェック
			if (args.size() < 2) {
				throw CommandException(this->name(), __FILE__, __LINE__, "command required.");
			}

			// 引数をスペースでつなげて一つの文字列に整形
			std::stringstream cmd;
			cmd << args[1];
			for (int i = 2; i < args.size(); i++) {
				cmd << " " << args[i];
			}

			// system実行
			std::string cinBuf;
			while (std::getline(std::cin, cinBuf)) {
				std::system((cmd.str() + " \"" + cinBuf + "\"").c_str());
			}
		}
		// シェル解釈無しで実行（exec実行）
		else {
			// TODO : Win32API CreateProcessで実装予定（Linuxはexecでforkと併用する必要あり。）

			throw CommandException(this->name(), __FILE__, __LINE__, "Not yet supported, Please use -s.");
		}

		return 0;
	}

}
}
}