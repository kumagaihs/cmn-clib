#include <iostream>
#include <regex>

#include "cmn-tools/Command.hpp"
#include "cmn-tools/CommandException.hpp"
#include "cmn-tools/commands/GrepCommand.hpp"

extern "C" {
#include "cmnclib.h"
}

namespace cmnclib {
namespace tools {
namespace command {

	int GrepCommand::execute(std::vector<std::string>& args) {
		if (args.size() < 1) {
			throw CommandException(this->name(), __FILE__, __LINE__, "keyword required.");
		}

		CmnStringBuffer *buf = CmnStringBuffer_Create("");

		// ファイル指定あり
		if (args.size() >= 2) {
			for (int i = 1; i < args.size(); i++) {
				// ファイル読み込み
				if (CmnFile_ReadAllText(args[i].c_str(), buf) == NULL) {
					throw CommandException(this->name(), __FILE__, __LINE__, "Failed read file, file=" + args[i]);
				}

				// 行単位に分解
				CmnStringList *list = CmnStringList_Create();
				if (CmnString_SplitLine(list, buf->string) == NULL) {
					throw CommandException(this->name(), __FILE__, __LINE__, "Out of memory.");
				}

				// フィルタリング＆出力
				for (int line = 0; line < list->size; line++) {
					output(CmnStringList_Get(list, line), args[0], args[i], line + 1);
				}
				CmnStringList_Free(list);
			}
		}
		// ファイル指定なし（標準入力からインプット）
		else {
			// 標準入力から読み込み
			std::string cinBuf;
			while (std::getline(std::cin, cinBuf)) {
				output(cinBuf.c_str(), args[0], "", 0);
			}
		}

		return 0;
	}

	void GrepCommand::output(const char *lineStr, const std::string &keyword, const std::string &fileName, int line) {
		std::cmatch cmatch;
		if (std::regex_search(lineStr, cmatch, std::regex(keyword))) {
			// ファイル名・行数出力
			if (!fileName.empty()) {
				std::cout << fileName << "(" << line << "):";
			}
			// 対象行出力
			std::cout << lineStr << std::endl;
		}
	}

}
}
}