#include <iostream>
#include "cmn-tools/Command.hpp"
#include "cmn-tools/CommandException.hpp"
#include "cmn-tools/commands/SetclipCommand.hpp"

extern "C" {
#include "cmnclib.h"
}

namespace cmnclib {
namespace tools {
namespace command {

	int SetclipCommand::execute(std::vector<std::string>& args) {
		// �N���b�v�{�[�h�ɐݒ肷��f�[�^�̏����i���������� or �W�����́j
		CmnStringBuffer *buf = CmnStringBuffer_Create("");
		if (args.size() > 0) {
			for (int i = 0; i < args.size(); i++) {
				std::string str = args[i];
				if (i > 0) {
					CmnStringBuffer_Append(buf, " ");
				}
				CmnStringBuffer_Append(buf, str.c_str());
			}
		}
		else {
			char readTmp[4096];
			while (true) {
				std::cin.read(readTmp, sizeof(readTmp) - 1);
				readTmp[std::cin.gcount()] = '\0';
				CmnStringBuffer_Append(buf, readTmp);

				if (std::cin.eof()) {
					break;
				}
			}
		}

		// �N���b�v�{�[�h�փf�[�^�Z�b�g
		if (CmnWin32Clipboard_SetString(buf) < 0) {
			throw CommandException(this->name(), __FILE__, __LINE__, "Cannot open the Clipboard");
		}

		return 0;
	}

}
}
}