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

		// �V�F�����߂���Ŏ��s�isystem���s�j
		if (args[0] == "-s") {
			// �����`�F�b�N
			if (args.size() < 2) {
				throw CommandException(this->name(), __FILE__, __LINE__, "command required.");
			}

			// �������X�y�[�X�łȂ��Ĉ�̕�����ɐ��`
			std::stringstream cmd;
			cmd << args[1];
			for (int i = 2; i < args.size(); i++) {
				cmd << " " << args[i];
			}

			// system���s
			std::string cinBuf;
			while (std::getline(std::cin, cinBuf)) {
				std::system((cmd.str() + " \"" + cinBuf + "\"").c_str());
			}
		}
		// �V�F�����ߖ����Ŏ��s�iexec���s�j
		else {
			// TODO : Win32API CreateProcess�Ŏ����\��iLinux��exec��fork�ƕ��p����K�v����B�j

			throw CommandException(this->name(), __FILE__, __LINE__, "Not yet supported, Please use -s.");
		}

		return 0;
	}

}
}
}