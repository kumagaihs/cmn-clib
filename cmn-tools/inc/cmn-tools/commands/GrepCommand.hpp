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
	std::string summary() override  { return "�w��t�@�C���������͕W�����͂̓��e�������Ɉ�v����s�݂̂Ƀt�B���^�����O���܂��B"; }
	std::string help() override  { return summary() + "\n" + R"(
<<< command >>>
grep keyword [file-name...]

<<< option >>>
  keyword
        ���o����������B���̏����Ɉ�v���Ȃ��s�͖��������B
        keyword�͐��K�\���iC++�W�� regex_match�j������Ƃ��ĉ��߂���B

  file-name
        �����񌟍��Ώۂ̃t�@�C���B�����w��\�B
        �ȗ������ꍇ�͕W�����͂��C���v�b�g�Ƃ���B
)"; }

	int execute(std::vector<std::string>& args);

private:
	void output(const char *lineStr, const std::string &keyword, const std::string &fileName, int line);

};

}
}
}

#endif
