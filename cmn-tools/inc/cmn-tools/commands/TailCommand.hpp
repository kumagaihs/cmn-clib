#ifndef _TAIL_COMMAND_HPP
#define _TAIL_COMMAND_HPP

#include "cmn-tools/Command.hpp"

namespace cmnclib {
namespace tools {
namespace command {

class TailCommand : public Command {

public:
	std::string name() override { return "tail"; }
	std::string summary() override  { return "�t�@�C���̖�����\������"; }
	std::string help() override  { return summary() + "\n" + R"(
<<< command >>>
�i���j������
tail [-n lines] [-f] target

�w��t�@�C���̖�����\������B

<<< option >>>
  -n
        �\������s�����w�肷��B�ȗ������ꍇ��10�s�\���ƂȂ�

  -f
        �t�@�C�����Ď����X�V���ꂽ���e��\������

  target
        �Ώۃt�@�C��
)"; }

	int execute(std::vector<std::string>& args);

};

}
}
}

#endif
