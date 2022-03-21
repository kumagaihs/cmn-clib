#ifndef _XARGS_COMMAND_HPP
#define _XARGS_COMMAND_HPP

#include "cmn-tools/Command.hpp"

namespace cmnclib {
namespace tools {
namespace command {

class XargsCommand : public Command {

public:
	std::string name() override { return "xargs"; }
	std::string summary() override  { return "�W�����͂̃f�[�^�Ŏw��̃R�}���h���J��Ԃ����s���܂��B"; }
	std::string help() override  { return summary() + "\n" + R"(
<<< command >>>
�i���j������
xargs [-s] command [command-options...]

�W�����͂����s�ǂݎ��A�ǂݎ�����f�[�^���Ō�̈����ɕt�^����command�����s����B
�����W�����͂̃f�[�^���Ȃ��Ȃ�܂ŌJ��Ԃ����s����B

<<< option >>>
  -s
        �T�u�V�F���Ŏ��s����B�p�C�v�⃊�_�C���N�g���̃V�F���@�\�𗘗p�\�Ƃ���B

  command
        ���s����R�}���h

  command-options
        command�ɕt�^��������B�����w��\�B

)"; }

	int execute(std::vector<std::string>& args);

};

}
}
}

#endif
